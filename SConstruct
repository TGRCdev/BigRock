import platform

sourcepaths = [Dir("src/"), Dir("src/data/"), Dir("src/mesh/"), Dir("src/data/tools")]
licenses = {"glm":[File('thirdparty/glm/copying.txt')], "glm-aabb":[File('thirdparty/cpm-glm-aabb/LICENSE')], "flatbuffers":[File('thirdparty/flatbuffers/LICENSE.txt')], "bigrock":[File('LICENSE')]}

def positive_validator(key, value, env):
    if int(value) <= 0:
        print("ERROR: " + key + " must be greater than 0. (value: " + str(value) + ")")
        Exit(1)

opts = Variables()
opts.Add(EnumVariable('target', 'The build target', 'debug', ['release', 'debug'], ignorecase=2))
opts.Add(EnumVariable('bits', 'The target bits', '', ['32', '64', '']))
opts.Add(BoolVariable('use_mingw', 'Set to \'yes\' to use MinGW on Windows machines.', False))
opts.Add(EnumVariable('build_type', 'The type of build to perform.', 'objects', ['objects', 'schemas', 'static', 'shared', 'dynamic', 'test'], ignorecase=2))
opts.Add(BoolVariable('make_dir', 'If \'yes\', constructs a library directory under \'lib/\' with the headers and the built library.', False))
opts.Add(PathVariable('glm_dir', 'The location of the GLM library headers and binaries to link with.', '', PathVariable.PathAccept))
opts.Add(PathVariable('glm_includes', 'The location of the headers for GLM to use.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_dir', 'The location of the FlatBuffers binaries, headers and the flatc compiler.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_includes', 'The location of the FlatBuffers headers directory.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_libs', 'The location of the FlatBuffers binaries directory.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatc_path', 'The path to the FlatBuffers schema compiler.', '', PathVariable.PathAccept))
opts.Add(BoolVariable('use_doubles', 'If \'yes\', uses double precision numbers for positions and isovalues.', False))
opts.Add('max_cell_depth', 'The max depth that terrain cells are allowed to subdivide to.', 24, positive_validator)

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
    env = Environment(TARGET_ARCH = target_arch, tools = ['mingw'])
    env['ENV']['PATH'] += ';C:\\Program Files\\mingw-w64\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin'
else:
    env = Environment(TARGET_ARCH = target_arch)

opts.Update(env)

if env['glm_dir'] == '':
    if platform.system() == 'Windows':
        env['glm_dir'] = 'C:\\Program Files (x86)\\glm'
if env['glm_includes'] == '' and env['glm_dir'] != '':
    print("setting includes")
    env['glm_includes'] = Dir(env['glm_dir'] + '/include')
env.Append(CPPPATH = [env['glm_includes']])


if env['flatbuffers_dir'] == '':
    if platform.system() == 'Windows':
        env['flatbuffers_dir'] = 'C:\\Program Files (x86)\\flatbuffers'
        if env['flatbuffers_includes'] == '':
            env['flatbuffers_includes'] = env['flatbuffers_dir'] + '/include'
        if env['flatbuffers_libs'] == '':
            env['flatbuffers_libs'] = env['flatbuffers_dir'] + '/lib'

if env['flatc_path'] == '':
    if platform.system() == 'Windows':
        env['flatc_path'] = env['flatbuffers_dir'] + '/bin/flatc'
    else:
        env['flatc_path'] = 'flatc'
env.Append(CPPPATH = [env['flatbuffers_includes']], LIBPATH = [env['flatbuffers_libs']], LIBS = 'flatbuffers')

if env['use_doubles']:
    env.Append(CPPDEFINES = 'BR_USE_DOUBLE_PRECISION')

## This only works half of the time and it's really god damn annoying
# conf = Configure(env)
# if not conf.CheckCXXHeader('glm/glm.hpp'):
# 	print("ERROR: The OpenGL Mathematics library must be available to build the library.")
# 	print("Please download and install GLM to build this library. ( https://glm.g-truc.net/ )")
# 	Exit(1)

env['bits'] = bits

if env['CC'] == 'cl':
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['/EHsc', '/DEBUG:FULL'], LINKFLAGS = ['/DEBUG:FULL'])

    if env['bits'] == '32':
        env.Append(LINKFLAGS = ['/MACHINE:X86'])
    else:
        env.Append(LINKFLAGS = ['/MACHINE:X64'])
else:
    #env.Append(CCFLAGS = ['-ansi'])
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['-g'])
    
    if env['bits'] == '32':
        env.Append(CCFLAGS = ['-m32'])
    else:
        env.Append(CCFLAGS = ['-m64'])
    
    env.Append(CCFLAGS = '-std=c++11')

env.Append(CPPDEFINES = ['GLM_FORCE_CXX11', ('BR_MAX_CELL_DEPTH', env['max_cell_depth'])])

sources = []
for path in sourcepaths:
    sources += Glob(str(path) + "/*.cpp")

targetname = 'bin/BigRock' + env['bits']
if env['target'] == 'debug':
    targetname += '-Debug'

# Setup schemas
schemas = Glob("src/data/schemas/*.fbs")
sch = []
for schema in schemas:
    sch += env.Command(str(schema)[:-4] + '_generated.h', schema, '"' + env['flatc_path'] + env['PROGSUFFIX'] + '" -o src/data/schemas -I src/data/schemas --cpp $SOURCE')

lib = None
if env['build_type'] == 'static':
    lib = Requires(env.StaticLibrary(target = targetname, source = sources), sch)
elif env['build_type'] == 'dynamic' or env['build_type'] == 'shared':
    lib = Requires(env.SharedLibrary(target = targetname, source = sources), sch)
elif env['build_type'] == 'test':
    Export('env', 'sources')
    Requires(env.SConscript('test/SCSub'), sch)
elif env['build_type'] == 'objects':
    Requires(env.Object(source=sources), sch)

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

Help(opts.GenerateHelpText(env))