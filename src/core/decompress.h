#pragma once
#include "common/restrict.h"
#include <stdlib.h>
#include <stdint.h>
size_t decompress(const uint8_t *compressed, size_t compressedSize, uint8_t **decompressed);