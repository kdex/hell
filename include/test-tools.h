#pragma once
#include "compress.h"
#include "decompress.h"
#include "types.h"
#include <stdlib.h>
int testDecompress(const u8 *compressed, const u8 *decompressedExpected, size_t expectedSize);
int compare(const u8 *actual, const u8 *expected, size_t actualSize, size_t expectedSize);
int fail(const char *format, ...);