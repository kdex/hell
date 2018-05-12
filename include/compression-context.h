#pragma once
#include "allocation.h"
#include "header.h"
typedef struct {
	Allocation *restrict allocation;
	Header *restrict small;
	Header *restrict large;
	const u8 *restrict uncompressed;
	u16 uncompressedSize;
} CompressionContext;
void initCompressionContext(CompressionContext *restrict context, const u8 *restrict uncompressed, size_t uncompressedSize);
void freeCompressionContext(CompressionContext *restrict context);