import os.path

#conary
from build import use
from conarycfg import ConfigFile, STRING, STRINGDICT, BOOL
import deps.deps

# XXX hack -- need a better way to add to list of config types
FLAGSENSE = 2222

class SubArchConfig(ConfigFile):
    defaults = {
	'name'	                : [ STRING, None ],
	'buildName'	        : [ STRING, None ],
        'subsumes'              : [ STRING, ''   ],
	'buildRequired'	        : [ BOOL,   True ],
	'shortDoc'	        : [ STRING, '' ],
	'longDoc'	        : [ STRING, '' ] 
    } 


class ArchConfig(ConfigFile):

    requiredArchProps = ['bits32', 'bits64', 'LE', 'BE'] 

    defaults = {
	'name'	                : [ STRING, None ],
	'buildName'	        : [ STRING, None ],
	'shortDoc'	        : [ STRING, '' ],
	'longDoc'	        : [ STRING, '' ],
        'archProp'              : [ STRINGDICT, {} ],
    } 


    def configLine(self, line, file = "override", lineno = '<No line>'):
	line = line.strip()
        if line and line[0] == '[' and line[-1] == ']':
            self.setSection(line[1:-1])
            return
        if self.section:
            self.sections[self.section].configLine(line, file, lineno)
        else:
            ConfigFile.configLine(self, line, file, lineno)

    def setSection(self, sectionName):
        if sectionName not in self.sections:
            self.sections[sectionName] = SubArchConfig()
        self.section = sectionName

    def __init__(self, name, path):
	ConfigFile.__init__(self)
        self.section = ''
        self.sections = {}
        filePath = os.path.join(path, name) 
        self.read(filePath)
        self.name = name
	if sorted(self.archProp.iterkeys()) != sorted(self.requiredArchProps):
	    raise RuntimeError, \
		    ('Arch %s must specify arch properties %s using the'
		     ' archProp directive' % (self.name,
		     ', '.join(sorted(self.requiredArchProps))))


    def addArchFlags(self):
        use.Arch._addFlag(self.name, archProps = self.archProp)
                                     
        for subArchName in self.sections:
            subArch = self.sections[subArchName]
            subArch.name = subArchName
            subArch.subsumes = [x.strip() for x in subArch.subsumes.split(',')]
            use.Arch[self.name]._addFlag(subArch.name,
                                         subsumes=subArch.subsumes)
            if subArch.buildName and subArch.buildName != subArch.name:
                use.Arch[self.name]._addAlias(subArch.name, subArch.buildName)



class UseFlagConfig(ConfigFile):
    defaults = {
	'name'	                : [ STRING, None ],
        'sense'                 : [ FLAGSENSE, deps.deps.FLAG_SENSE_PREFERRED ],
	'buildName'	        : [ STRING, None ],
	'buildRequired'	        : [ BOOL,   True ],
	'shortDoc'	        : [ STRING, '' ],
	'longDoc'	        : [ STRING, '' ],
    }

    def __init__(self, name, path):
	ConfigFile.__init__(self)
        filePath = os.path.join(path, name)
	# Hack to allow old-style config files to be parsed
        contents = open(filePath).read().strip()
        if contents.strip() in ('disallowed', 'preferred', 'prefernot', 
                                                           'required'):
            self.configLine('installFlavor %s' % contents, filePath, 1)
        else:
            self.read(filePath)
        if self.name is None:
            self.name = name
        assert(self.name == name)

    def setValue(self, key, val, type=None, file="override"):
	if type == None:
	    type = self.types[key]
        if type == FLAGSENSE:
            val = val.lower()
            if val == "disallowed":
                sense = deps.deps.FLAG_SENSE_DISALLOWED
            elif val == "preferred":
                sense = deps.deps.FLAG_SENSE_PREFERRED
            elif val == "prefernot":
                sense = deps.deps.FLAG_SENSE_PREFERNOT
            elif val == "required":
                sense = deps.deps.FLAG_SENSE_REQUIRED
            else:
                raise ParseError, ("%s: unknown use value %s") % (filePath, val)
            self.__dict__[key] = sense
        else:
            ConfigFile.setValue(self, key, val, type, file)

    def displayKey(self, key, value, type, out):
        if type == FLAGSENSE:
            if value == deps.deps.FLAG_SENSE_DISALLOWED:
                out.write('%s: %s\n' % (key, "disallowed"))
            elif value == deps.deps.FLAG_SENSE_PREFERRED:
                out.write('%s: %s\n' % (key, "preferred"))
            elif value == deps.deps.FLAG_SENSE_PREFERNOT:
                out.write('%s: %s\n' % (key, "prefernot"))
            elif value == deps.deps.FLAG_SENSE_REQUIRED:
                out.write('%s: %s\n' % (key, "required"))
        else:
            ConfigFile.displayKey(self, key, value, type, out)

    def toDepFlag(self):
        return (self.name, self.sense)

    def addUseFlag(self):
        use.Use._addFlag(self.name, required=self.buildRequired) 
        if self.buildName and self.buildName != self.name:
            use.Use._addAlias(self.name, self.buildName)

class FlavorConfig:
    """
    contains information reflecting the use flags specified by the 
    use and arch paths
    """

    def __init__(self, useDir, archDir):
        self.flags = {}
        self.arches = {}
        if useDir and os.path.exists(useDir):
            for flag in os.listdir(useDir):
		if os.path.isfile(os.path.join(useDir, flag)):
		    self.flags[flag] = UseFlagConfig(flag, useDir)
        if archDir and os.path.exists(archDir):
            for arch in os.listdir(archDir):
		if os.path.isfile(os.path.join(archDir, arch)):
		    self.arches[arch] = ArchConfig(arch, archDir)

    def toDependency(self, override=None):
        useFlags = deps.deps.DependencySet()
        flags = [x.toDepFlag() for x in self.flags.values() ] 

        useFlags.addDep(deps.deps.UseDependency, 
                        deps.deps.Dependency("use", flags))
        if override:
            useFlags.union(override,
                           mergeType = deps.deps.DEP_MERGE_TYPE_OVERRIDE)
        return useFlags

    def populateBuildFlags(self):
        for flag in self.flags.itervalues():
            flag.addUseFlag()
        for arch in self.arches.itervalues():
            arch.addArchFlags()

	# These are the required arch properties, every architecture
        # must specify these values
	use.Arch._setArchProps('bits32', 'bits64', 'LE', 'BE')
