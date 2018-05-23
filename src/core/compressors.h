#pragma once
#include "common/restrict.h"
#include "core/compression-context.h"
#include "core/compression-mode.h"
#include <stdlib.h>
u16 compressUncompressed(CompressionContext *RESTRICT context, u16 size, const u8 *RESTRICT payload);
u16 compressFillByte(CompressionContext *RESTRICT context, u16 size, u8 byte);
u16 compressFillBytes(CompressionContext *RESTRICT context, u16 size, u8 byteA, u8 byteB);
u16 compressFillIncrementalSequence(CompressionContext *RESTRICT context, u16 size, u8 seed);
u16 compressCopy(CompressionContext *RESTRICT context, u16 size, CompressionMode mode, u16 copyOffset);