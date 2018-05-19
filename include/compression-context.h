#pragma once
#include "allocation.h"
#include "header.h"
typedef struct {
	Allocation *restrict allocation;
	Header *restrict small;
	Header *restrict large;
	u16 stashSize;
	u16 stashOffset;
} CompressionContext;
void initCompressionContext(CompressionContext *restrict context);
void stash(CompressionContext *restrict context, size_t position);
void flushStash(CompressionContext *restrict context, const u8 *uncompressed);
void terminateCompressionContext(CompressionContext *context);
void freeCompressionContext(CompressionContext *restrict context);