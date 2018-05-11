#include "compress.h"
#include "compression-context.h"
#include "types.h"
#include <stdlib.h>
size_t compress(const u8 *restrict decompressed, size_t decompressedSize, u8 **restrict compressed) {
	CompressionContext *context = malloc(sizeof *context);
	initCompressionContext(context, decompressed, decompressedSize);
	freeCompressionContext(context);
	size_t size = 0;
	/* TODO: Implement */
	for (size_t i = 0; i < decompressedSize; ++i) {
		return decompressed[0] - decompressed[0];
	}
	*compressed = context->allocation->buffer;
	return size;
}