#include "compression-context.h"
#include "constants.h"
#include "compressors.h"
#include "header.h"
#include "types.h"
void initCompressionContext(CompressionContext *restrict context) {
	*context = (CompressionContext) {
		.allocation = malloc(sizeof (Allocation)),
		.small = malloc(sizeof (Header)),
		.large = malloc(sizeof (Header)),
		.stashSize = 0,
		.stashOffset = 0
	};
	initAllocation(context->allocation);
	initSmallHeader(context->small);
	initLargeHeader(context->large);
}
void stash(CompressionContext *restrict context, size_t stashOffset) {
	if (!context->stashSize) {
		context->stashOffset = stashOffset;
	}
	++context->stashSize;
}
void flushStash(CompressionContext *restrict context, const u8 *uncompressed) {
	if (context->stashSize) {
		compressUncompressed(context, context->stashSize, uncompressed + context->stashOffset);
		context->stashSize = 0;
	}
}
void terminateCompressionContext(CompressionContext *context) {
	resize(context->allocation, context->allocation->written + 1);
	context->allocation->buffer[context->allocation->written] = END;
	++context->allocation->written;
}
void freeCompressionContext(CompressionContext *restrict context) {
	freeAllocation(context->allocation);
	free(context->small);
	free(context->large);
	free(context);
}