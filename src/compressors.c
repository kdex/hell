#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
size_t compressUncompressedU8(CompressionContext *context, const u8 *payload, u16 size) {
	Header *header = NULL;
	size_t *offset = &context->allocation->offset;
	size_t startOffset = *offset;
	if (size <= context->small->maxStorage) {
		header = context->small;
	}
	else {
		header = context->large;
	}
	const u8 firstByte = makeFirstByte(header, UNCOMPRESSED, size);
	/* An additional byte is used to store the end */
	const size_t compressedSize = header->size + size + 1;
	reserve(context->allocation, compressedSize);
	context->allocation->buffer[startOffset] = firstByte;
	context->allocation->buffer[startOffset + 1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		context->allocation->buffer[startOffset + header->size + i] = payload[i];
	}
	context->allocation->buffer[startOffset + header->size + size] = END;
	*offset += compressedSize;
	return compressedSize;
}