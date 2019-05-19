sourcepaths = [Dir("src/"), Dir("src/data/"), Dir("src/mesh/"), Dir("src/data/tools")]
licenses = {"glm":[File('thirdparty/glm/copying.txt')], "glm-aabb":[File('thirdparty/cpm-glm-aabb/LICENSE')], "bigrock":[File('LICENSE')]}

def positive_validator(key, value, env):
    if int(value) <= 0:
        print("ERROR: " + key + " must be greater than 0. (value: " + str(value) + ")")
        Exit(1)

opts = Variables()
opts.Add(EnumVariable('target', 'The build target', 'debug', ['release', 'debug'], ignorecase=2))
opts.Add(EnumVariable('bits', 'The target bits', '', ['32', '64', '']))
opts.Add(BoolVariable('use_mingw', 'Set to \'yes\' to use MinGW on Windows machines.', False))
opts.Add(EnumVariable('build_type', 'The type of build to perform.', 'objects', ['objects', 'static', 'shared', 'dynamic', 'test'], ignorecase=2))
opts.Add(BoolVariable('make_dir', 'If \'yes\', constructs a library directory under \'lib/\' with the headers and the built library.', False))
opts.Add(PathVariable('glm_dir', 'The location of the GLM library headers and binaries to link with.', 'C:\\Program Files (x86)\\glm', PathVariable.PathAccept))
opts.Add(PathVariable('glm_includes', 'The location of the headers for GLM to use.', '', PathVariable.PathAccept))

bits = ARGUMENTS.get('bits', '')
if not bits: # Use host bits as default bits
    host_arch = DefaultEnvironment()['HOST_ARCH']
    if host_arch:
        arches = {
            'x86' : '32',
            'x86_64' : '64',
            'i386' : '32',
            'amd64' : '64',
            'emt64' : '64',
            'ia64' : '64'
        }
        bits = arches[host_arch]
    else:
        bits = '64'

target_arch = 'x86_64' if bits == '64' else 'x86'

if ARGUMENTS.get('use_mingw', False) == 'yes':
    env = Environment(TARGET_ARCH = target_arch , tools = ['mingw'])
else:
    env = Environment(TARGET_ARCH = target_arch)
opts.Update(env)
if env['glm_includes'] == '':
    env['glm_includes'] = Dir(env['glm_dir'] + '/include')
env.Append(CPPPATH = [env['glm_includes']])

conf = Configure(env)
if not conf.CheckCXXHeader('glm/glm.hpp'):
	print("ERROR: The OpenGL Mathematics library must be available to build the library.")
	print("Please download and install GLM to build this library. ( https://glm.g-truc.net/ )")
	Exit(1)

env['bits'] = bits

if env['CC'] == 'cl':
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['/EHsc', '/DEBUG:FULL'], LINKFLAGS = ['/DEBUG:FULL'])

    if env['bits'] == '32':
        env.Append(LINKFLAGS = ['/MACHINE:X86'])
    else:
        env.Append(LINKFLAGS = ['/MACHINE:X64'])
else:
    if env['use_mingw']:
        env.Append()
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['-g'])
    
    if env['bits'] == '32':
        env.Append(CCFLAGS = ['-m32'])
    else:
        env.Append(CCFLAGS = ['-m64'])

sources = []
for path in sourcepaths:
    sources += Glob(str(path) + "/*.cpp")

targetname = 'bin/BigRock' + env['bits']
if env['target'] == 'debug':
    targetname += '-Debug'

lib = None
if env['build_type'] == 'static':
    lib = env.StaticLibrary(target = targetname, source = sources)
elif env['build_type'] == 'dynamic' or env['build_type'] == 'shared':
    lib = env.SharedLibrary(target = targetname, source = sources)
elif env['build_type'] == 'test':
    Export('env', 'sources', 'conf')
    env.SConscript('test/SCSub')
else:
    env.Object(source=sources)

if env['make_dir']:
    if lib:
        headers = []
        for path in sourcepaths:
            env.Install('lib/include/bigrock/' + str(path)[4:], Glob(str(path) + "/*.hpp"))
        for libname in licenses.keys():
            env.Install('lib/licenses/' + libname, licenses[libname])
        env.Install('lib/lib', lib)
    else:
        print("WARN: make_dir only works when building a library")