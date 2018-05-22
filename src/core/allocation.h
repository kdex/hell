#pragma once
#include "common/types.h"
#include <stdlib.h>
typedef struct AllocationInternals AllocationInternals;
typedef struct {
	size_t written;
	u8 *buffer;
	AllocationInternals *internals;
} Allocation;
void initAllocation(Allocation *allocation);
void resize(Allocation * const allocation, size_t newSize);
void reserve(Allocation * const allocation, size_t bytes);
void freeAllocation(Allocation *allocation);