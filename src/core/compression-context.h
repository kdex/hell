#pragma once
#include "core/allocation.h"
#include "core/header.h"
typedef struct AllocationInternals AllocationInternals;
typedef struct {
	Allocation *restrict allocation;
	Header *restrict small;
	Header *restrict large;
	AllocationInternals *restrict internals;
} CompressionContext;
void initCompressionContext(CompressionContext *restrict context);
void stash(CompressionContext *restrict context, size_t position);
void flushStash(CompressionContext *restrict context, const u8 *uncompressed);
void terminateCompressionContext(CompressionContext *context);
void freeCompressionContext(CompressionContext *restrict context);