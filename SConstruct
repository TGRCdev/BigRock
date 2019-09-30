import platform, os, sys

sourcepaths = [Dir("src/"), Dir("src/data/"), Dir("src/mesh/"), Dir("src/data/tools"), Dir("src/data/actions"), Dir("src/stl")]
licenses = {"glm":[File('thirdparty/glm/copying.txt')], "glm-aabb":[File('thirdparty/cpm-glm-aabb/LICENSE')], "flatbuffers":[File('thirdparty/flatbuffers/LICENSE.txt')], "bigrock":[File('LICENSE')]}

def positive_validator(key, value, env):
    if int(value) <= 0:
        print("ERROR: " + key + " must be greater than 0. (value: " + str(value) + ")")
        Exit(1)

is_64bit = platform.machine().endswith("64")
if is_64bit:
    host_bits = '64'
else:
    host_bits = '32'

if not ARGUMENTS.get("bits", ''):
    print('"bits" not set, assuming host bits: ' + host_bits)

opts = Variables()
opts.Add(EnumVariable('target', 'The build target', 'debug', ['release', 'debug'], ignorecase=2))
opts.Add(EnumVariable('bits', 'The target bits', host_bits, ['32', '64']))
opts.Add(EnumVariable('build_type', 'The type of build to perform.', 'objects', ['objects', 'schemas', 'static', 'shared', 'dynamic', 'tests'], ignorecase=2))
opts.Add(BoolVariable('make_dir', 'If \'yes\', constructs a library directory under \'lib/\' with the headers and the built library.', False))
opts.Add(PathVariable('glm_dir', 'The location of the GLM library headers and binaries to link with.', os.environ.get("GLM_DIR", ''), PathVariable.PathAccept))
opts.Add(PathVariable('glm_includes', 'The location of the headers for GLM to use.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_dir', 'The location of the FlatBuffers binaries, headers and the flatc compiler.', os.environ.get("FLATBUFFERS_DIR", ''), PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_includes', 'The location of the FlatBuffers headers directory.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatbuffers_libs', 'The location of the FlatBuffers binaries directory.', '', PathVariable.PathAccept))
opts.Add(PathVariable('flatc_path', 'The path to the FlatBuffers schema compiler.', '', PathVariable.PathAccept))
opts.Add('max_cell_depth', 'The max depth that terrain cells are allowed to subdivide to.', 24, positive_validator)
opts.Add(PathVariable('cache', 'If defined, caches build files here. Mostly for Travis CI and AppVeyor.', '', PathVariable.PathAccept))

target_arch = 'x86_64' if host_bits == '64' else 'x86'

env = Environment(TARGET_ARCH = target_arch)

opts.Update(env)
unknown = opts.UnknownVariables()
if unknown:
    print("WARNING: Unknown variables - " + str(unknown.keys()))
    Exit(1)

if platform.system() == "Windows" and env['glm_dir'] == '':
    print("ERR: Could not find GLM directory. Please set 'glm_dir' in your scons call, or define GLM_DIR in your environment variables.")
    Exit(1)

if env['glm_includes'] == '' and env['glm_dir'] != '':
    env['glm_includes'] = Dir(env['glm_dir'] + '/include')
env.Append(CPPPATH = [env['glm_includes']])

if env['flatbuffers_dir'] == '':
    if platform.system() == 'Windows':
        print("ERR: Could not find FlatBuffers directory. Please set 'flatbuffers_dir' in your scons call, or define FLATBUFFERS_DIR in your environment variables.")
        Exit(1)
if env['flatbuffers_includes'] == '':
    if platform.system() == "Windows":
        env['flatbuffers_includes'] = env['flatbuffers_dir'] + '/include'
if env['flatbuffers_libs'] == '':
    if platform.system() == "Windows":
        env['flatbuffers_libs'] = env['flatbuffers_dir'] + '/lib'

if env['flatc_path'] == '':
    if platform.system() == 'Windows':
        env['flatc_path'] = env['flatbuffers_dir'] + '/bin/flatc'
    else:
        env['flatc_path'] = 'flatc'
env.Append(CPPPATH = [env['flatbuffers_includes']], LIBPATH = [env['flatbuffers_libs']], LIBS = 'flatbuffers')

if env['cache']:
    print("Caching enabled at directory \"" + env['cache'] + "\"")
    Mkdir(env['cache'])
    CacheDir(env['cache'])

## This only works half of the time and it's really god damn annoying
# conf = Configure(env)
# if not conf.CheckCXXHeader('glm/glm.hpp'):
# 	print("ERROR: The OpenGL Mathematics library must be available to build the library.")
# 	print("Please download and install GLM to build this library. ( https://glm.g-truc.net/ )")
# 	Exit(1)

if env['target'] == 'release':
    env.Append(CPPDEFINES = "NDEBUG")

if env['CC'] == 'cl':
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['/EHsc', '/DEBUG:FULL', '/Z7'], LINKFLAGS = ['/DEBUG:FULL'])
    else:
        env.Append(CCFLAGS = ['/O2'])
    
else:
    #env.Append(CCFLAGS = ['-ansi'])
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['-g'])
    else:
        env.Append(CCFLAGS = ['-O2'])
    
    if env['bits'] == '32':
        env.Append(CCFLAGS = ['-m32'], LINKFLAGS = ['-m32'])
    else:
        env.Append(CCFLAGS = ['-m64'], LINKFLAGS = ['-m64'])
    
    env.Append(CXXFLAGS = '-std=c++11', LIBS=['pthread'])

if platform.system() == 'Windows':
    env.Append(LIBS=['winmm'])

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
    this_sch = env.Command(str(schema)[:-4] + '_generated.h', schema, '"' + env['flatc_path'] + '" -o src/data/schemas -I src/data/schemas --cpp $SOURCE')
    # get dependencies
    contents = schema.get_text_contents()
    next_include = contents.find('include "')
    pos = next_include
    while(next_include != -1):
        pos = next_include + len('include "')
        #print("{} has dependency {}".format(os.path.basename(schema.abspath), contents[pos:contents.find('"', pos)]))
        env.Depends(this_sch, os.path.dirname(schema.abspath) + "/" + contents[pos:contents.find('"', pos)])
        pos = contents.find('"', pos)
        next_include = contents.find('include "', pos)
    
    sch += this_sch

lib = None
if env['build_type'] == 'static':
    lib = Requires(env.StaticLibrary(target = targetname, source = sources), sch)
elif env['build_type'] == 'dynamic' or env['build_type'] == 'shared':
    lib = Requires(env.SharedLibrary(target = targetname, source = sources), sch)
elif env['build_type'] == 'tests':
    Export('env', 'sources', 'opts')
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
        print("WARN: make_dir only works when building a library. Scons will do nothing otherwise.")

Help(opts.GenerateHelpText(env))