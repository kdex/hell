#include "compression-context.h"
#include "allocation.h"
#include "compressors.h"
#include "header.h"
#include "common/constants.h"
#include "common/types.h"
struct AllocationInternals {
	u16 stashSize;
	u16 stashOffset;
};
void initCompressionContext(CompressionContext *restrict context) {
	*context = (CompressionContext) {
		.allocation = malloc(sizeof (Allocation)),
		.small = malloc(sizeof (Header)),
		.large = malloc(sizeof (Header)),
		.internals = malloc(sizeof (AllocationInternals))
	};
	initAllocation(context->allocation);
	initSmallHeader(context->small);
	initLargeHeader(context->large);
	*context->internals = (AllocationInternals) {
		.stashSize = 0,
		.stashOffset = 0
	};
}
void stash(CompressionContext *restrict context, size_t position) {
	if (!context->internals->stashSize) {
		context->internals->stashOffset = position;
	}
	++context->internals->stashSize;
}
void flushStash(CompressionContext *restrict context, const u8 *uncompressed) {
	if (context->internals->stashSize) {
		compressUncompressed(context, context->internals->stashSize, uncompressed + context->internals->stashOffset);
		context->internals->stashSize = 0;
	}
}
void terminateCompressionContext(CompressionContext *context) {
	resize(context->allocation, context->allocation->written + 1);
	context->allocation->buffer[context->allocation->written] = END;
	++context->allocation->written;
}
void freeCompressionContext(CompressionContext *restrict context) {
	freeAllocation(context->allocation);
	freeHeader(context->small);
	freeHeader(context->large);
	free(context->internals);
	free(context);
}