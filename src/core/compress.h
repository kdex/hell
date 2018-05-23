#pragma once
#include <stdlib.h>
#include <stdint.h>
uint32_t compress(const uint8_t *uncompressed, uint32_t uncompressedSize, uint8_t **compressed);