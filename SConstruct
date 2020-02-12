env = Environment()

debug = ARGUMENTS.get('mode', 'release')
if debug == 'debug':
    env['CCFLAGS'] = '-g'
    print '\n----build in debug mode----\n'
else:
    print '\n----build in release mode----\n'


LIBS = ''
common_libs = ['gtest', 'pthread']
env.Append( LIBS = common_libs )
source = Glob('art/*.cpp') + Glob('art/*.h') + ['Main.cpp'] 

env.Program('Main', source, LIBS, LIBPATH=['/usr/lib/'])
