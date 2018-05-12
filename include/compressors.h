#pragma once
#include "compression-context.h"
#include <stdlib.h>
size_t compressUncompressed(CompressionContext *restrict context);
size_t compressFillByte(CompressionContext *restrict context, u8 byte);
size_t compressFillBytes(CompressionContext *restrict context, u8 byteA, u8 byteB);