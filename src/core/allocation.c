#include "core/allocation.h"
#include "common/constants.h"
#include "core/util.h"
#include <stdlib.h>
struct AllocationInternals {
	size_t size;
};
void initAllocation(Allocation *allocation) {
	*allocation = (Allocation) {
		.written = 0,
		.buffer = NULL,
		.internals = malloc(sizeof (AllocationInternals))
	};
	*allocation->internals = (AllocationInternals) {
		.size = 0
	};
}
void resize(Allocation * const allocation, size_t newSize) {
	if (allocation->internals->size != newSize) {
		allocation->buffer = realloc(allocation->buffer, newSize);
		allocation->internals->size = newSize;
	}
}
size_t availableSpace(const Allocation *allocation) {
	return allocation->internals->size - allocation->written;
}
void reserve(Allocation * const allocation, size_t bytes) {
	if (availableSpace(allocation) < bytes) {
		const size_t newSize = minimum(MAX_PAYLOAD, maximum(allocation->written + bytes, allocation->internals->size * 2));
		resize(allocation, newSize);
	}
}
void freeAllocation(Allocation *allocation) {
	free(allocation->internals);
	free(allocation->buffer);
	free(allocation);
}