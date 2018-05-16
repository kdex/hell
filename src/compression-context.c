#include "compression-context.h"
#include "header.h"
#include "types.h"
void initCompressionContext(CompressionContext *restrict context) {
	*context = (CompressionContext) {
		.allocation = malloc(sizeof (Allocation)),
		.small = malloc(sizeof (Header)),
		.large = malloc(sizeof (Header))
	};
	initAllocation(context->allocation);
	initSmallHeader(context->small);
	initLargeHeader(context->large);
}
void freeCompressionContext(CompressionContext *restrict context) {
	freeAllocation(context->allocation);
	free(context->small);
	free(context->large);
	free(context);
}
size_t terminateCompressionContext(CompressionContext *context) {
	resize(context->allocation, context->allocation->offset + 1);
	context->allocation->buffer[context->allocation->offset] = END;
	return 1;
}