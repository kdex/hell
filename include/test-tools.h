#pragma once
#include "compress.h"
#include "compressors.h"
#include "decompress.h"
#include "types.h"
#include <stdlib.h>
int test(size_t (*fn)(const u8 *payload, size_t payloadSize, u8 **destination), const u8 *payload, size_t payloadSize, const u8 *expected, size_t expectedSize);
int compare(const u8 *actual, const u8 *expected, size_t actualSize, size_t expectedSize);
int fail(const char *format, ...);