#pragma once
#include "compression-context.h"
#include <stdlib.h>
size_t compressUncompressedU8(CompressionContext *context, const u8 *payload, u16 size);