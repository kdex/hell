#include "allocation.h"
#include "util.h"
#include <stdlib.h>
void initAllocation(Allocation *allocation) {
	*allocation = (Allocation) {
		.size = 0,
		.offset = 0,
		.buffer = NULL
	};
}
void freeAllocation(Allocation *allocation) {
	free(allocation->buffer);
	free(allocation);
}
void resize(Allocation * const allocation, size_t newSize) {
	allocation->buffer = realloc(allocation->buffer, newSize);
	allocation->size = newSize;
}
void reserve(Allocation * const allocation, size_t minLimit) {
	const size_t maxLimit = 2 * minLimit;
	if (allocation->size < maxLimit) {
		/* TODO: Limit max allocation to something reasonable */
		const size_t newSize = max(maxLimit, allocation->size * 2);
		resize(allocation, newSize);
	}
}