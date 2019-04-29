sourcepaths = ["src/", "src/terrain/", "src/math/", "src/terrain/data/", "src/terrain/mesh/"]

opts = Variables()
opts.Add(EnumVariable('target', 'The build target', 'debug', ['release', 'debug'], ignorecase=2))
opts.Add(EnumVariable('bits', 'The target bits', '32', ['32', '64']))
opts.Add(EnumVariable('use_mingw', 'Set to \'yes\' to use MinGW on Windows machines.', 'no', ['yes', 'no'], ignorecase=2))
opts.Add(EnumVariable('build_type', 'The type of build to perform.', 'objects', ['objects', 'static', 'shared', 'dynamic'], ignorecase=2))
opts.Add(EnumVariable('make_dir', 'If \'yes\', constructs a library directory under \'lib/\' with the headers and the built library.', 'no', ['yes', 'no'], ignorecase=2))

if ARGUMENTS.get('use_mingw', 'no') == 'yes':
    env = DefaultEnvironment(options = opts, tools = ['mingw'])
else:
    env = DefaultEnvironment(options = opts)

if env['CC'] == 'cl':
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['/EHsc', '/DEBUG'])

    if env['bits'] == '32':
        env.Append(LINKFLAGS = ['/MACHINE:X86'])
    else:
        env.Append(LINKFLAGS = ['/MACHINE:X64'])
else:
    if env['target'] == 'debug':
        env.Append(CCFLAGS = ['-g'])
    
    if env['bits'] == '32':
        env.Append(CCFLAGS = ['-m32'])
    else:
        env.Append(CCFLAGS = ['-m64'])

sources = []
for path in sourcepaths:
    sources += Glob(path + "*.cpp")

targetname = 'bin/BigRock' + env['bits']
if env['target'] == 'debug':
    targetname += '-Debug'

lib = None
if env['build_type'] == 'static':
    lib = env.StaticLibrary(target = targetname, source = sources)
elif env['build_type'] == 'dynamic' or env['build_type'] == 'shared':
    lib = env.SharedLibrary(target = targetname, source = sources)
else:
    env.Object(source=sources)

if env['make_dir'] == 'yes':
    if lib:
        headers = []
        for path in sourcepaths:
            env.Install('lib/include/bigrock/' + path[4:], Glob(path + "*.hpp"))
        env.Install('lib/bin', lib)
    else:
        print("WARN: make_dir only works when building a library")