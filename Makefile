.PHONY: clean debug release tests pre-build
EXECUTABLES = bin
HAL_SOURCES = src/api.c src/hal.c src/decompress.c
TEST_DECOMPRESS_SOURCES = src/api.c test/decompress.c src/decompress.c
MAKE_LUT_SOURCES = src/make-lut.c
LUT = include/lut.h
SHARED_FLAGS =\
	-Iinclude\
	-Wall\
	-Wextra\
	-Wshadow\
	-Wpedantic
RELEASE_FLAGS =\
	-O3\
	-s\
	-fomit-frame-pointer\
	-ffunction-sections -fdata-sections -Wl,--gc-sections\
	-fno-unwind-tables -fno-asynchronous-unwind-tables\
	-fno-math-errno\
	-fno-unroll-loops\
	-fmerge-all-constants\
	-fno-ident\
	-ffast-math\
	-fdelete-null-pointer-checks\
	-Wl,-z,norelro\
	-Wl,--hash-style=gnu\
	-Wl,--build-id=none
DEBUG_FLAGS =\
	-DDEBUG\
	-fsanitize=address\
	-fsanitize=undefined\
	-ggdb\
	-O0
all: release
debug: MODE_FLAGS=$(DEBUG_FLAGS)
debug: hal test-decompress
release: MODE_FLAGS=$(RELEASE_FLAGS)
release: pre-build
release: hal test-decompress
pre-build: $(EXECUTABLES)
	$(MKDIR $(EXECUTABLES))
hal: $(HAL_SOURCES) $(LUT)
	$(CC) -o $(EXECUTABLES)/hal $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $(HAL_SOURCES)
$(LUT): $(MAKE_LUT_SOURCES)
	$(CC) -o $(EXECUTABLES)/make-lut $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $(MAKE_LUT_SOURCES)
	./$(EXECUTABLES)/make-lut $(LUT)
tests: MODE_FLAGS=$(DEBUG_FLAGS)
tests: pre-build
tests: test-decompress
test-decompress: $(TEST_DECOMPRESS_SOURCES) $(LUT)
	$(CC) -o $(EXECUTABLES)/test-decompress $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $$(pkg-config --cflags --libs check) $(TEST_DECOMPRESS_SOURCES)
clean:
	$(RM) $(LUT)
	$(RMDIR $(EXECUTABLES))