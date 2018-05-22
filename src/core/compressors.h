#pragma once
#include "core/compression-context.h"
#include "core/compression-mode.h"
#include <stdlib.h>
u16 compressUncompressed(CompressionContext *restrict context, u16 size, const u8 *restrict payload);
u16 compressFillByte(CompressionContext *restrict context, u16 size, u8 byte);
u16 compressFillBytes(CompressionContext *restrict context, u16 size, u8 byteA, u8 byteB);
u16 compressFillIncrementalSequence(CompressionContext *restrict context, u16 size, u8 seed);
u16 compressCopy(CompressionContext *restrict context, u16 size, CompressionMode mode, u16 copyOffset);