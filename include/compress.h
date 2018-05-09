#pragma once
#include <stdlib.h>
#include <stdint.h>
size_t compress(const uint8_t *decompressed, size_t decompressedSize, uint8_t **compressed);