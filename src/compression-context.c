#include "compression-context.h"
#include "header.h"
#include "types.h"
void initCompressionContext(CompressionContext *context) {
	*context = (CompressionContext) {
		.allocation = malloc(sizeof (Allocation)),
		.small = malloc(sizeof (Header)),
		.large = malloc(sizeof (Header))
	};
	initAllocation(context->allocation);
	initSmallHeader(context->small);
	initLargeHeader(context->large);
}
void freeCompressionContext(CompressionContext *context) {
	freeAllocation(context->allocation);
	free(context->small);
	free(context->large);
	free(context);
}