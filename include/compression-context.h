#pragma once
#include "allocation.h"
#include "header.h"
typedef struct {
	Allocation *restrict allocation;
	Header *restrict small;
	Header *restrict large;
} CompressionContext;
void initCompressionContext(CompressionContext *restrict context);
void freeCompressionContext(CompressionContext *restrict context);