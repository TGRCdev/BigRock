import os

# Variables
cpppath = ['src/', './']
arches = {
    'x86' : '32',
    'i386' : '32',
    'amd64' : '64',
    'emt64' : '64',
    'x86_64' : '64',
    'ia64' : '64'
}

denv = DefaultEnvironment()
if ARGUMENTS.get('platform', '') == 'windows':
    default_arch = denv['HOST_ARCH']
elif ARGUMENTS.get('platform', '') == 'osx':
    default_arch = 'x86_64'
else:
    default_arch = 'x86'

# Setup options
opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable('platform', 'The target compilation platform', '', allowed_values=['', 'windows', 'linux', 'osx'], ignorecase=2))
opts.Add(EnumVariable('target', 'The target of compilation', 'debug', allowed_values=['debug', 'release'], ignorecase=2))
opts.Add(EnumVariable('build_type', 'What kind of build to run (Shared, dynamic and static refer to library types)', 'objects', allowed_values=['shared', 'dynamic', 'static', 'objects', 'tests'], ignorecase=2))
opts.Add(EnumVariable('use_mingw', 'Whether or not to use MinGW for compilation (Windows only)', 'no', allowed_values=['no', 'yes'], ignorecase=2))
opts.Add(EnumVariable('bits', 'The system bits to target', arches[default_arch], allowed_values=['32', '64'], ignorecase=2))

# Setup environment
VariantDir('bin', 'src', duplicate=0)
target_arch = 'x86' if ARGUMENTS.get('bits', arches[default_arch]) == '32' else 'x86_64'

if ARGUMENTS.get('platform', '') == 'windows' and ARGUMENTS.get('use_mingw', 'no') == 'yes':
    env = Environment(CPPPATH = cpppath, tools = ['mingw'], TARGET_ARCH = target_arch)
else:
    env = Environment(CPPPATH = cpppath, TARGET_ARCH = target_arch)
opts.Update(env)

sources = []
# Get sources
for path in cpppath:
    sources += Glob(path + "*.cpp")

# Check options
if env['platform'] == 'windows':
    if env['use_mingw'] == 'yes':
        env.Append(CCFLAGS = ['-std=gnu++98', '-pedantic-errors'], LINKFLAGS = ['-static-libstdc++', '-static-libgcc'])
        if env['target'] == 'debug':
            env.Append(CCFLAGS = '-g')
        if env['bits'] == '32':
            env.Append(CCFLAGS = '-m32')
        else:
            env.Append(CCFLAGS = '-m64')
    else:
        if env['target'] == 'debug':
            env.Append(LINKFLAGS = '/DEBUG:FULL', CCFLAGS = '/EHsc')
elif env['platform'] == 'osx' or env['platform'] == 'linux':
    env.Append(CCFLAGS = ['-std=c++98', '-pedantic-errors'])
    if env['target'] == 'debug':
        env.Append(CCFLAGS = '-g')
    
    if env['bits'] == '32':
        env.Append(CCFLAGS = '-m32')
    else:
        env.Append(CCFLAGS = '-m64')
else:
    print("A valid target platform must be defined. (windows, linux, osx)")
    Exit(1)

# Build
if env['build_type'] == 'objects':
    env.Object(source = sources)
elif env['build_type'] == 'shared' or env['build_type'] == 'dynamic':
    env.SharedLibrary(source = sources)
elif env['build_type'] == 'static':
    env.StaticLibrary(source = sources)
elif env['build_type'] == 'tests':
    for test in Glob("tests/*.cpp"):
        target_name = str(test)[5:-4]
        env.Program(source = sources + [test], target = 'bin/tests/' + target_name)