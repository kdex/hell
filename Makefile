.PHONY: all clean debug release tests pre-build
BIN = bin
HELL_SOURCES = src/hell.c src/decompress.c
TEST_DECOMPRESS_SOURCES = test/decompress.c src/decompress.c
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
debug: $(BIN)/hell $(BIN)/test-decompress
release: MODE_FLAGS=$(RELEASE_FLAGS)
release: pre-build
release: $(BIN)/hell $(BIN)/test-decompress
pre-build:
	$(MKDIR $(BIN))
$(BIN)/hell: $(HELL_SOURCES) $(LUT)
	$(CC) -o $(BIN)/hell $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $(HELL_SOURCES)
$(LUT): $(MAKE_LUT_SOURCES)
	$(CC) -o $(BIN)/make-lut $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $(MAKE_LUT_SOURCES)
	$(BIN)/make-lut $(LUT)
tests: MODE_FLAGS=$(DEBUG_FLAGS)
tests: pre-build
tests: $(BIN)/test-decompress
$(BIN)/test-decompress: $(TEST_DECOMPRESS_SOURCES) $(LUT)
	$(CC) -o $(BIN)/test-decompress $(SHARED_FLAGS) $(MODE_FLAGS) $(CFLAGS) $$(pkg-config --cflags --libs check) $(TEST_DECOMPRESS_SOURCES)
clean:
	$(RM) $(LUT)
	$(RMDIR $(BIN))