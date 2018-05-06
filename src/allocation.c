#include "allocation.h"
#include <stdlib.h>
size_t max(size_t a, size_t b) {
	return a > b ? a : b;
}
void resize(struct Allocation * const allocation, size_t newSize) {
	*allocation->block = realloc(*allocation->block, newSize);
	allocation->size = newSize;
}
void reserve(struct Allocation * const allocation, size_t minLimit) {
	const size_t maxLimit = 2 * minLimit;
	if (allocation->size < maxLimit) {
		/* TODO: Limit max allocation to something reasonable */
		const size_t newSize = max(maxLimit, allocation->size * 2);
		resize(allocation, newSize);
	}
}