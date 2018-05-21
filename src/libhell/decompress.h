#pragma once
#include <stdlib.h>
#include <stdint.h>
size_t decompress(const uint8_t *restrict compressed, size_t compressedSize, uint8_t **restrict decompressed);