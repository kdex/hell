#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
size_t compressUncompressedU8(CompressionContext *restrict context, const u8 *restrict payload, u16 size) {
	Header *header;
	if (size <= context->small->capacity) {
		header = context->small;
	}
	else {
		header = context->large;
	}
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
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