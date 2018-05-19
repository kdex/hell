project('hell', 'c',
	version: '0.0.1',
	default_options: [
		'c_std=c11',
		'buildtype=release',
		'b_ndebug=if-release',
		'warning_level=3'
	]
)
name = meson.project_name()
buildType = get_option('buildtype')
cc = meson.get_compiler('c')
cflags = []
if buildType == 'debug'
	flags = [
		'-DDEBUG',
		'-ggdb',
		'-Wrestrict'
	]
	foreach flag : flags
		if cc.has_argument(flag)
			cflags += flag
		endif
	endforeach
endif
if buildType == 'release'
	flags = [
		'-s',
		'-fomit-frame-pointer',
		'-ffunction-sections',
		'-fdata-sections',
		'-fno-unwind-tables',
		'-fno-asynchronous-unwind-tables',
		'-fno-math-errno',
		'-fno-unroll-loops',
		'-fmerge-all-constants',
		'-fno-ident',
		'-ffast-math',
		'-fdelete-null-pointer-checks',
	]
	foreach flag : flags
		if cc.has_argument(flag)
			cflags += flag
		endif
	endforeach
endif
foreach argument : [
	'-Wno-newline-eof',
	'-Wshadow'
]
	if cc.has_argument(argument)
		cflags += argument
	endif
endforeach
includes = include_directories('include')
testDependencies = files(
	'src/allocation.c',
	'src/constants.c',
	'src/compress.c',
	'src/compression-context.c',
	'src/compressors.c',
	'src/decompress.c',
	'src/header.c',
	'src/io.c',
	'src/test-tools.c',
	'src/util.c',
)
makeLut = executable('make-lut', [
	'src/make-lut/main.c',
	'src/constants.c'
],
	c_args: cflags,
	include_directories: includes
)
lut = custom_target('Look-up table',
	output: 'lut.c',
	command: [makeLut, '@OUTPUT@']
)
executable(name, [
	'src/allocation.c',
	'src/constants.c',
	'src/compress.c',
	'src/compression-context.c',
	'src/compressors.c',
	'src/decompress.c',
	'src/header.c',
	'src/hell/main.c',
	'src/io.c',
	'src/util.c'
], lut,
	c_args: cflags,
	include_directories: includes,
	install: true
)
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
	'src/constants.c',
	'src/decompress.c',
	'src/util.c'
], lut,
	c_args: cflags,
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