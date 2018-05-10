#include "allocation.h"
#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
size_t compressUncompressedU8(CompressionContext *context, const u8 *payload, u16 size) {
	Header *header = NULL;
	if (size <= context->small->maxStorage) {
		header = context->small;
	}
	else if (size <= context->large->maxStorage) {
		header = context->large;
	}
	const u8 firstByte = makeFirstByte(header, UNCOMPRESSED, size);
	const size_t compressedSize = size + header->size + 1;
	/* An additional byte is used to store the end */
	reserve(context->allocation, compressedSize);
	context->allocation->buffer[0] = firstByte;
	context->allocation->buffer[1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		/* TODO: `0` is obivously not supposed to be a constant */
		context->allocation->buffer[0 + header->size + i] = payload[i];
	}
	context->allocation->buffer[header->size + size] = END;
	return compressedSize;
}