import glob

files = glob.glob('src/*.cpp')
files = [x.replace("src", "obj") for x in files]

env = Environment(CPPPATH = ['include'])

debug = ARGUMENTS.get('debug', 0)
if int(debug):
	env.Append(CCFLAGS = '-g')

env.BuildDir('obj','src', duplicate = 0)
env.Program('monday',files,LIBS=['alleg-4.9.4','a5_iio','a5_font','a5_ttf','lua'])
