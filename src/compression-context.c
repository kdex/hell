#include "compression-context.h"
#include "header.h"
#include "types.h"
void initCompressionContext(CompressionContext *restrict context, const u8 *restrict uncompressed, size_t uncompressedSize) {
	*context = (CompressionContext) {
		.allocation = malloc(sizeof (Allocation)),
		.small = malloc(sizeof (Header)),
		.large = malloc(sizeof (Header)),
		.uncompressed = uncompressed,
		.uncompressedSize = uncompressedSize
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