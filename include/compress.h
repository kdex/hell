#pragma once
#include <stdlib.h>
#include <stdint.h>
size_t compress(const uint8_t *restrict uncompressed, size_t uncompressedSize, uint8_t **restrict compressed);