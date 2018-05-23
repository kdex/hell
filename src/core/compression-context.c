#include "core/compression-context.h"
#include "common/constants.h"
#include "common/restrict.h"
#include "common/types.h"
#include "core/allocation.h"
#include "core/compressors.h"
#include "core/header.h"
struct AllocationInternals {
	u16 stashSize;
	u16 stashOffset;
};
void initCompressionContext(CompressionContext *RESTRICT context) {
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
void stash(CompressionContext *RESTRICT context, u16 position) {
	if (!context->internals->stashSize) {
		context->internals->stashOffset = position;
	}
	++context->internals->stashSize;
}
void flushStash(CompressionContext *RESTRICT context, const u8 *uncompressed) {
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
void freeCompressionContext(CompressionContext *RESTRICT context) {
	freeAllocation(context->allocation);
	freeHeader(context->small);
	freeHeader(context->large);
	free(context->internals);
	free(context);
}