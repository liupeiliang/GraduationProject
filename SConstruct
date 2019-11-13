env = Environment()
LIBS = ''
common_libs = ['gtest', 'pthread']
env.Append( LIBS = common_libs )
source = Glob('art/*.cpp') + Glob('art/*.h') + ['Main.cpp'] 

env.Program('Main', source, LIBS, LIBPATH=['/usr/lib/'])