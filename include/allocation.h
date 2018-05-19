#pragma once
#include "types.h"
#include <stdlib.h>
typedef struct {
	size_t size;
	size_t written;
	u8 *buffer;
} Allocation;
void initAllocation(Allocation *allocation);
void freeAllocation(Allocation *allocation);
void resize(Allocation * const allocation, size_t newSize);
void reserve(Allocation * const allocation, size_t minLimit);