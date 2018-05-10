#pragma once
#include "allocation.h"
#include "header.h"
typedef struct {
	Allocation *allocation;
	Header *small;
	Header *large;
} CompressionContext;
void initCompressionContext(CompressionContext *context);
void freeCompressionContext(CompressionContext *context);