project('hell', 'c', default_options: [
	'c_std=c11',
	'buildtype=release',
	'b_ndebug=if-release',
	'b_pgo=use',
	'warning_level=3',
])
includes = include_directories('include')
testDependencies = files(
	'src/allocation.c',
	'src/decompress.c',
	'src/test-tools.c'
)
makeLut = executable('make-lut', 'src/make-lut.c',
	include_directories: includes
)
lut = custom_target('Look-up table',
	output: 'lut.h',
	command: [makeLut, '@OUTPUT@']
)
executable('hell', [
	'src/allocation.c',
	'src/hell.c',
	'src/decompress.c'
], lut,
	include_directories: includes
)
subdir('test')