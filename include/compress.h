#pragma once
#include <stdlib.h>
#include <stdint.h>
size_t compress(const uint8_t *restrict decompressed, size_t decompressedSize, uint8_t **restrict compressed);