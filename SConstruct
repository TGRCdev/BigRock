sourcepaths = ["src/", "src/terrain/", "src/math/", "src/terrain/data/", "src/terrain/mesh/"]

opts = Variables()
opts.Add(EnumVariable('make_lib_folder', 'Set to \'yes\' to generate a folder with the headers and the lib included (under \'lib\').', 'no', ['yes', 'no'], ignorecase=2))
opts.Add(EnumVariable('use_mingw', 'Set to \'yes\' to use MinGW on Windows machines.', 'no', ['yes', 'no'], ignorecase=2))
opts.Add(PathVariable('boost_path', 'The path to the boost installation to use for compilation.', "C:/Program Files/boost/boost_1_69_0", PathVariable.PathIsDir))
opts.Add('build_test', 'The name of the test program to build', '')

if ARGUMENTS.get('use_mingw', 'no') == 'yes':
    env = DefaultEnvironment(options = opts, tools = ['mingw'])
else:
    env = DefaultEnvironment(options = opts)

env.Append(CPPPATH = env['boost_path'])

print("CC = " + env['CC'])
if env['CC'] == 'cl':
    env.Append(CCFLAGS = ['/EHsc', '/O2', '/Ox'])
else:
    env.Append(CCFLAGS = ['-g'])

sources = []
for path in sourcepaths:
    sources += Glob(path + "*.cpp")

if env['make_lib_folder'] == 'yes':
    lib = env.StaticLibrary(target="bin/libBigRock", source=sources)
    env.Install('lib/lib', lib)
    for path in sourcepaths:
        env.Install("lib/include/bigrock/" + path, Glob(path + "*.hpp"))
elif env['build_test'] != '':
    sources = Glob("src/test/" + env['build_test'] + ".cpp") + sources;
    env.Program(source=sources)
else:
    env.Object(source=sources)