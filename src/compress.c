#include "compress.h"
#include "compression-context.h"
#include "types.h"
#include <stdlib.h>
size_t compress(const u8 *decompressed, size_t decompressedSize, u8 **compressed) {
	CompressionContext *context = malloc(sizeof *context);
	initCompressionContext(context);
	freeCompressionContext(context);
	size_t size = 0;
	/* TODO: Implement */
	for (size_t i = 0; i < decompressedSize; ++i) {
		return decompressed[0] - decompressed[0];
	}
	*compressed = context->allocation->buffer;
	return size;
}