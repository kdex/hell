#pragma once
#include "compression-context.h"
#include "compression-mode.h"
#include <stdlib.h>
size_t compressUncompressed(CompressionContext *restrict context);
size_t compressFillByte(CompressionContext *restrict context, u8 byte);
size_t compressFillBytes(CompressionContext *restrict context, u8 byteA, u8 byteB);
size_t compressFillIncrementalSequence(CompressionContext *restrict context, u8 seed);
size_t compressCopy(CompressionContext *restrict context, CompressionMode mode, u16 copyOffset);