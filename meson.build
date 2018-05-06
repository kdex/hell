project('hell', 'c',
	version: '0.0.1',
	default_options: [
		'c_std=c11',
		'buildtype=release',
		'b_ndebug=if-release',
		'b_pgo=use',
		'warning_level=3',
		'b_sanitize=address,undefined'
	]
)
name = meson.project_name()
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
#executable(name, [
#	'src/allocation.c',
#	'src/decompress.c',
#	'src/hell.c'
#], lut,
#	include_directories: includes,
#	install: true
#)
publicHeaders = []
foreach header : [
	'decompress',
	name
]
	publicHeaders += join_paths('include', header + '.h')
endforeach
install_headers(
	publicHeaders,
	subdir: name
)
libraries = both_libraries(name, [
	'src/allocation.c',
	'src/decompress.c'
],
	include_directories: includes,
	install: true
)
import('pkgconfig').generate(
	libraries: [libraries],
	subdirs: name,
	version: meson.project_version(),
	name: name,
	description: '(De-)compression tools for video games by HAL Laboratory'
)
subdir('test')