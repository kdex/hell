#pragma once
#include <stdlib.h>
#include <stdint.h>
uint32_t decompress(const uint8_t *compressed, uint32_t compressedSize, uint8_t **decompressed);