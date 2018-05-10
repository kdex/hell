#pragma once
#include "compression-context.h"
#include <stdlib.h>
size_t compressUncompressedU8(CompressionContext *restrict context, const u8 *restrict payload, u16 size);