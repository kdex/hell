#pragma once
#include "types.h"
#include <stdlib.h>
struct Allocation {
	u8 **block;
	size_t size;
};
void resize(struct Allocation * const allocation, size_t newSize);
void reserve(struct Allocation * const allocation, size_t minLimit);