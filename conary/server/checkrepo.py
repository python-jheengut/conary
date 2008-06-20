#!/usr/bin/python
# -*- mode: python -*-
#
# Copyright (c) 2004-2008 rPath, Inc.
#
# This program is distributed under the terms of the Common Public License,
# version 1.0. A copy of this license should have been distributed with this
# source file in a file called LICENSE. If it is not present, the license
# is always available at http://www.rpath.com/permanent/licenses/CPL-1.0.
#
# This program is distributed in the hope that it will be useful, but
# without any warranty; without even the implied warranty of merchantability
# or fitness for a particular purpose. See the Common Public License for
# full details.
#

import os
import sys

thisFile = sys.modules[__name__].__file__
thisPath = os.path.dirname(thisFile)
if thisPath:
    mainPath = thisPath + "/../.."
else:
    mainPath = "../.."
mainPath = os.path.realpath(mainPath)
sys.path.insert(0, mainPath)

from conary.lib import options
from conary.server import schema
from conary.local import schema as depSchema
from conary.lib.cfg import CfgBool, CfgInt, CfgPath
from conary.lib import log
from conary.repository.netrepos import netserver, instances
from conary import dbstore

class Checker:
    """ base class for checking/fixing an issue """
    def __init__(self, cfg, fix = False):
        self.cfg = cfg
        self._fix = fix
        self._db = None
        self._status = None
        self._alwaysfix = False
        self._postinit()
    def _postinit(self):
        pass
    
    def getDB(self):
        if self._db:
            return self._db
        (driver, database) = self.cfg.repositoryDB
        self._db = dbstore.connect(database, driver)
        schema.setupTempTables(self._db)
        depSchema.setupTempDepTables(self._db)
        return self._db
    
    def commit(self, db = None):
        if db is None:
            db = self._db
        assert(db)
        #db.commit()
        db.rollback()
        
    def check(self):
        return True
    def repair(self):
        if not self._status:
            log.debug("no errors detected on check run, nothing to fix")
            return True
        return self.fix()
    def fix(self):
        return True
    
    def run(self):
        log.info("Running:  %s", self.__doc__)
        ret = self.check()
        if self._alwaysfix or (not ret and self._fix):
            ret = self.repair()
        log.info("%-7s: %s\n", ["FAIL", "Success"][int(bool(ret))], self.__doc__)
        return ret

class CheckAcls(Checker):
    """ acls and permission caches checks """
    def check(self):
        db = self.getDB()
        cu = db.cursor()
        log.info("checking Permissions acls")
        cu.execute("""
        select p.permissionId, p.userGroupId, ug.userGroup, i.item, l.label, ugap.c, checker.c
        from Permissions as p
        join (
            select permissionId, count(*) as c
            from UserGroupAllPermissions
            group by permissionId ) as ugap using(permissionId)
        join (
            select permissionId, count(*) as c from (
                select Permissions.permissionId as permissionId,
                       Instances.instanceId as instanceId
                from Instances
                join Nodes using(itemId, versionId)
                join LabelMap using(itemId, branchId)
                join Permissions on
                    Permissions.labelId = 0 or Permissions.labelId = LabelMap.labelId
                join CheckTroveCache on
                    Permissions.itemId = CheckTroveCache.patternId and
                    Instances.itemId = CheckTroveCache.itemId
                 ) as perms
             group by permissionId ) as checker using (permissionId)
        join UserGroups as ug using (userGroupId)
        join Items as i on p.itemId = i.itemId
        join Labels as l on p.labelId = l.labelId
        order by permissionId
        """)
        self._status = set()
        ret = True
        for (permissionId, roleId, role, trovePattern, label, crtCounter, newCounter) in cu:
            if crtCounter == newCounter:
                continue
            log.warning("acl(%d) (%s %s %s) caches %d entries instead of %d entries",
                        permissionId, role, label, trovePattern, crtCounter, newCounter)
            self._status.add((permissionId, roleId, role))
            ret = False
        if not ret:
            log.info("check fails with %d errors found", len(self._status))
        return ret

    def fix(self):
        from conary.repository.netrepos import accessmap
        db = self.getDB()
        ri = accessmap.RoleInstances(db)
        for (permissionId, roleId, role) in self._status:
            log.info("fixing permission cache for %s...", role)
            ri.updatePermissionId(permissionId, roleId)
        log.info("checking again to verify changes...")
        self._status = set()
        if not self.check():
            log.error("FAILED to fix the permissions cache. Unhandled error - contact rPath")
            db.rollback()
            return False
        self.commit()
        return True

class CheckLatest(Checker):
    """ LatestCache table rebuilding """
    def _postinit(self):
        self._alwaysfix = self._fix
    def check(self):
        log.debug("LatestCache table will be rebuilt is --fix is passed in")
        self._status = 1
        return True
    def fix(self):
        from conary.repository.netrepos import versionops
        db = self.getDB()
        latest = versionops.LatestTable(db)
        log.info("rebuilding the LatestCache table...")
        latest.rebuild()
        self.commit()
        return True

    
class CheckTroveInfo(Checker):
    """ checks for extra/erroneous troveinfo records """
    def check(self):
        db = self.getDB()
        cu = db.cursor()
        log.info("checking for extraneous troveinfo records")
        cu.execute(""" select instanceId, count(*)
            from Instances join TroveInfo using(instanceId)
            where Instances.isPresent = ?
            group by instanceId having count(*) > 0 """, instances.INSTANCE_PRESENT_MISSING)
        self._status = cu.fetchall()
        if self._status:
            log.warning("found %d non-present troves with troveinfo records", len(self._status))
            return False
        return True
    def fix(self):
        db = self.getDB()
        cu = db.cursor()
        log.info("removing troveinfo records for non-prsent troves...")
        schema.resetTable(cu, "tmpId")
        cu.execute(""" insert into tmpId(id)
            select distinct instanceId from Instances join TroveInfo using(instanceId)
            where Instances.isPresent = ? """, instances.INSTANCE_PRESENT_MISSING)
        cu.execute("delete from TroveInfo where instanceId in (select id from tmpId)")
        self.commit()
        return self.check()

# main program
class ServerConfig(netserver.ServerConfig):
    port                    = (CfgInt,  8000)
    sslCert                 = CfgPath
    sslKey                  = CfgPath
    useSSL                  = CfgBool
    def __init__(self, path="serverrc"):
	netserver.ServerConfig.__init__(self)
	self.read(path, exception=False)
        if self.tmpDir.endswith('/'):
            self.tmpDir = self.tmpDir[:-1]
    def check(self):
        if not self.contentsDir:
            log.error("contentsDir needs to be specified")
            return False
        if not self.tmpDir:
            log.error("tmpDir needs to be specified")
            return False
        if not os.path.isdir(self.tmpDir):
            log.error("%s needs to be a directory", self.tmpDir)
            return False
        if not os.access(self.tmpDir, os.R_OK | os.W_OK | os.X_OK):
            log.error("%s needs to allow full read/write access", self.tmpDir)
            return False
        if os.path.realpath(self.tmpDir) != self.tmpDir:
            log.error("tmpDir cannot include symbolic links")
            return False
        return True
    
def startLogging():
    import logging
    # set a format which is simpler for console use
    formatter = logging.Formatter('%(asctime)s %(message)s', datefmt = "%m-%d %H:%M")
    # tell the handler to use this format
    log.logger.handlers[0].setFormatter(formatter)
    log.setVerbosity(log.DEBUG)
    log.info("Logging system started")
    
def usage():
    print """ checks repository for data consistency
    Usage:
    %s [--fix] [--config-file repo.cnr] [--config 'name param'] checkname [checkname...]
    Valid check names are: ALL acls latest troveinfo
    """

def getServer(opts = {}, argv = sys.argv, cfgMap = {}):
    cfg = ServerConfig()
    cfgMap.update({
        'contents-dir'  : 'contentsDir',
	'db'	        : 'repositoryDB',
	'tmp-dir'       : 'tmpDir',
        'server-name'   : 'serverName'
        })
    
    opts["config"] = options.MULT_PARAM
    opts["config-file"] = options.ONE_PARAM

    try:
        argSet, otherArgs = options.processArgs(opts, cfgMap, cfg, usage, argv = argv)
    except options.OptionError, msg:
        print >> sys.stderr, msg
        sys.exit(1)

    if not cfg.check():
        raise RuntimeError("configuration file is invalid")

    (driver, database) = cfg.repositoryDB
    db = dbstore.connect(database, driver)
    log.info("checking schema version on %s://%s", driver, database)
    # if there is no schema or we're asked to migrate, loadSchema
    dbVersion = db.getVersion()
    # a more recent major is not compatible
    if dbVersion.major > schema.VERSION.major:
        log.error("code base too old for this repository database")
        log.error("repo=%s code=%s", dbVersion, schema.VERSION)
        sys.exit(-1)
    db.close()
    return (cfg, argSet, otherArgs[1:])

def main():
    opts =  {}
    opts["fix"] = options.NO_PARAM
    startLogging()
    cfg, opts, args = getServer(opts)
    
    doFix = opts.has_key("fix")
    if not args:
        args = ["troveinfo"]
    log.info("Starting tests\n")

    ret = {}
    all = False
    if "ALL" in acls:
        all = True
    if all or "acls" in args:
        ret["acls"] = CheckAcls(cfg, doFix).run()
    if all or "latest" in args:
        ret["latest"] = CheckLatest(cfg, doFix).run()
    if all or "troveinfo" in args:
        ret["troveinfo"] = CheckTroveInfo(cfg, doFix).run()
        
    return ret

if __name__ == '__main__':
    main()
    