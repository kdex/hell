#include "allocation.h"
#include "constants.h"
#include "util.h"
#include <stdlib.h>
void initAllocation(Allocation *allocation) {
	*allocation = (Allocation) {
		.size = 0,
		.written = 0,
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
size_t freeSpace(const Allocation *allocation) {
	return allocation->size - allocation->written;
}
void reserve(Allocation * const allocation, size_t bytes) {
	if (freeSpace(allocation) < bytes) {
		const size_t newSize = min(MAX_PAYLOAD, max(allocation->written + bytes, allocation->size * 2));
		resize(allocation, newSize);
	}
}