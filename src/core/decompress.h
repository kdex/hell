#pragma once
#include "common/restrict.h"
#include <stdint.h>
#include <stdlib.h>
uint32_t decompress(
	const uint8_t *RESTRICT compressed,
	uint32_t compressedSize,
	uint8_t **RESTRICT decompressed
);