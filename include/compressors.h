#pragma once
#include "compression-context.h"
#include <stdlib.h>
size_t compressUncompressed(CompressionContext *restrict context, u16 size);
size_t compressFillByte(CompressionContext *restrict context, u16 size, u8 byte);