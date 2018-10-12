#pragma once
#include "common/restrict.h"
#include <stdint.h>
#include <stdlib.h>
uint32_t compress(
	const uint8_t *RESTRICT uncompressed,
	uint32_t uncompressedSize,
	uint8_t **RESTRICT compressed
);