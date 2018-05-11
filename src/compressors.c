#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
size_t compressUncompressed(CompressionContext *restrict context, u16 size) {
	Header *header;
	if (size <= context->small->capacity) {
		header = context->small;
	}
	else {
		header = context->large;
	}
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	/* An additional byte (+ 1) is used to store the end */
	const size_t compressedSize = header->size + size + 1;
	reserve(context->allocation, compressedSize);
	context->allocation->buffer[startOffset] = makeFirstByte(header, UNCOMPRESSED, size);
	context->allocation->buffer[startOffset + 1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		context->allocation->buffer[startOffset + header->size + i] = context->uncompressed[i];
	}
	context->allocation->buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}
size_t compressFillByte(CompressionContext *restrict context, u16 size, u8 byte) {
	Header *header;
	if (size <= context->small->capacity) {
		header = context->small;
	}
	else {
		header = context->large;
	}
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	/*
	* Chunk structure:
	* [ Header ][ Byte value ][ END ]
	*/
	const size_t compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	context->allocation->buffer[startOffset] = makeFirstByte(header, FILL_BYTE, size);
	context->allocation->buffer[startOffset + 1] = size - 1;
	context->allocation->buffer[startOffset + header->size] = byte;
	context->allocation->buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}