#pragma once
#include "common/restrict.h"
#include "core/allocation.h"
#include "core/header.h"
typedef struct AllocationInternals AllocationInternals;
typedef struct {
	Allocation *RESTRICT allocation;
	Header *RESTRICT small;
	Header *RESTRICT large;
	AllocationInternals *RESTRICT internals;
} CompressionContext;
void initCompressionContext(CompressionContext *RESTRICT context);
void stash(CompressionContext *RESTRICT context, u16 position);
void flushStash(CompressionContext *RESTRICT context, const u8 *uncompressed);
void terminateCompressionContext(CompressionContext *context);
void freeCompressionContext(CompressionContext *RESTRICT context);