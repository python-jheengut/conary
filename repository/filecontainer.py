#
# Copyright (c) 2004 Specifix, Inc.
#
# This program is distributed under the terms of the Common Public License,
# version 1.0. A copy of this license should have been distributed with this
# source file in a file called LICENSE. If it is not present, the license
# is always available at http://www.opensource.org/licenses/cpl.php.
#
# This program is distributed in the hope that it will be useful, but
# without any waranty; without even the implied warranty of merchantability
# or fitness for a particular purpose. See the Common Public License for
# full details.
#
"""
Provides a file which can store multiple files inside of it.

A table of contents is utilized, and the caller can attach arbitrary
data to an entry in the table. No compression is used, but that could
be changed.

The data attached to table entries can be modified, but doing so isn't
particularly efficient (the entire table gets rewritten).

The last file added to the container can be erased from it. This is
to allow operations to be reverted.

The file format is::
  - magic
  - file format version
  - total # of bytes in file tables
  - file table entry 1
  - file table entry 2
  -                  .
  -                  .
  -                  .
  - file table entry N
  - file 1
  - file 2
  .
  .
  .
  - file N

Everything after the file format version is gzipped (so gzip magic
appears right after the file version).

Each file table entry looks like::

  lengt
  of entry (4 bytes), not including these 4 bytes
  length of file name (4 bytes)
  file name 
  file offset (4 bytes) (in the gzipped data)
  file size (4 bytes)
  length of arbitrary data (4 bytes)
  entries in file table (4 bytes)
  arbitrary file table data
"""

from repository import filecontents
import gzip
import os
import struct
from lib import util

FILE_CONTAINER_MAGIC = "\xEA\x3F\x81\xBB"
FILE_CONTAINER_VERSION = 2004092001
SEEK_SET = 0
SEEK_CUR = 1
SEEK_END = 2

class FileTableEntryFromFile1(object):

    __slots__ = [ "name", "offset", "size", "data", "src" ]

    def write(self, file):
	rc = (struct.pack("!i", len(self.name)) + self.name +
	      struct.pack("!i", self.offset) +
	      struct.pack("!i", self.size) +
	      struct.pack("!i", len(self.data)) + self.data)
	rc = struct.pack("!i", len(rc)) + rc
	assert(len(rc) == self.tableSize())
	return file.write(rc)

    def tableSize(self):
	return len(self.name) + len(self.data) + 20 

    def setOffset(self, new):
	self.offset = new

    def writeContents(self, dest):
	(fileObj, size) = self.src.getWithSize()
	dest.write(struct.pack("!HI", len(self.name), size))
	dest.write(self.name)
	util.copyfileobj(fileObj, dest)

    def __init__(self, f):
	# read the length of the entry
	size = f.read(4)
	(size,) = struct.unpack("!i", size)

	rest = f.read(size)

	# unpack the length of the file name
	(size,) = struct.unpack("!i", rest[0:4])
	i = 4
	# and the file name
	(self.name,) = struct.unpack("%ds" % size, rest[i:i+size])
	i = i + size
	# and the file offset
	(self.offset,) = struct.unpack("!i", rest[i:i+4])
	i = i+ 4
	# and the file size
	(self.size,) = struct.unpack("!i", rest[i:i+4])
	i = i+ 4
	# the length of the arbitrary data
	(size,) = struct.unpack("!i", rest[i:i+4])
	i = i + 4
	# and the arbitrary data
	(self.data,) = struct.unpack("%ds" % size, rest[i:])

class FileContainer:

    bufSize = 128 * 1024

    def readTable(self):
	magic = self.file.read(4)
	if len(magic) != 4 or magic != FILE_CONTAINER_MAGIC:
	    raise BadContainer, "bad magic"

	version = self.file.read(4)
	if len(version) != 4:
	    raise BadContainer, "invalid container version"
	version = struct.unpack("!I", version)[0]
	if version != FILE_CONTAINER_VERSION:
	    raise BadContainer, "unsupported file container version %d" % \
			version

	self.gzfile = gzip.GzipFile(None, "rb", None, self.file)

	self.contentsStart = self.gzfile.tell()
	self.next = self.contentsStart

    def close(self):
	if self.mutable:
	    self.rest.close()
	self.file = None
    
    def addFile(self, fileName, contents, tableData):
	assert(isinstance(contents, filecontents.FileContents))
	assert(self.mutable)

	(fileObj, size) = contents.getWithSize()
	self.rest.write(struct.pack("!HIH", len(fileName), size, 
			len(tableData)))
	self.rest.write(fileName)
	self.rest.write(tableData)
	util.copyfileobj(fileObj, self.rest)

    def getNextFile(self):
	assert(not self.mutable)

	eatCount = self.next - self.gzfile.tell()

	# in case the file wasn't completely read in (it may have
	# already been in the repository, for example)
	while eatCount > self.bufSize:
	    self.gzfile.read(self.bufSize)
	    eatCount -= self.bufSize
	self.gzfile.read(eatCount)

	name, tag, size = self.nextFile()

	if name is None:
	    return None

	fcf = util.NestedFile(self.gzfile, size)
	self.next = self.gzfile.tell() + size

	return (name, tag, fcf, size)

    def nextFile(self):
	nameLen = self.gzfile.read(8)
	if not len(nameLen):
	    return (None, None, None)

	nameLen, size, tagLen = struct.unpack("!HIH", nameLen)
	name = self.gzfile.read(nameLen)
	tag = self.gzfile.read(tagLen)
	return (name, tag, size)
	
    def __del__(self):
	if self.file:
	    self.close()

    def __init__(self, file):
        """
        Create a FileContainer object.
        
        @param file: an open python file object referencing the file
        container file on disk. If that file is empty (size 0) the
        file container is immediately initialized. A copy of the file
        is retained, so the caller may optionally close it.
        """
        
	# make our own copy of this file which nobody can close underneath us
	self.file = os.fdopen(os.dup(file.fileno()), file.mode)

	self.file.seek(0, SEEK_END)
	if not self.file.tell():
	    self.file.seek(SEEK_SET, 0)
	    self.file.truncate()
	    self.file.write(FILE_CONTAINER_MAGIC)
	    self.file.write(struct.pack("!I", FILE_CONTAINER_VERSION))

	    self.mutable = True
	    self.rest = gzip.GzipFile(None, "wb", 6, self.file)
	else:
	    self.file.seek(0, SEEK_SET)
	    try:
		self.readTable()
	    except:
		self.file.close()
		self.file = None
		raise
	    self.mutable = False
	
class BadContainer(Exception):

    pass
