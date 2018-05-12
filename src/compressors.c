#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
void chooseHeader(const CompressionContext *restrict context, Header **restrict header, u16 size) {
	if (size <= context->small->capacity) {
		*header = context->small;
	}
	else {
		*header = context->large;
	}
}
size_t compressUncompressed(CompressionContext *restrict context) {
	const u16 size = context->uncompressedSize;
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	Header *header;
	chooseHeader(context, &header, size);
	/*
	* Chunk structure:
	* [ Header ][ Uncompressed data ][ END ]
	*/
	const size_t compressedSize = header->size + size + 1;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, UNCOMPRESSED, size);
	buffer[startOffset + 1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		buffer[startOffset + header->size + i] = context->uncompressed[i];
	}
	buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}
size_t compressFillByte(CompressionContext *restrict context, u8 byte) {
	const u16 size = context->uncompressedSize;
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	Header *header;
	chooseHeader(context, &header, size);
	/*
	* Chunk structure:
	* [ Header ][ Byte value ][ END ]
	*/
	const size_t compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_BYTE, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = byte;
	buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}
size_t compressFillBytes(CompressionContext *restrict context, u8 byteA, u8 byteB) {
	const u16 size = context->uncompressedSize / 2;
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	Header *header;
	chooseHeader(context, &header, size);
	/*
	* Chunk structure:
	* [ Header ][ Byte A ][ Byte B ][ END ]
	*/
	const size_t compressedSize = header->size + 3;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_BYTES, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = byteA;
	buffer[startOffset + header->size + 1] = byteB;
	buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}
size_t compressFillIncrementalSequence(CompressionContext *restrict context, u8 seed) {
	const u16 size = context->uncompressedSize;
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	Header *header;
	chooseHeader(context, &header, size);
	/*
	* Chunk structure:
	* [ Header ][ Seed ][ END ]
	*/
	const size_t compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_INCREMENTAL_SEQUENCE, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = seed;
	buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}
size_t compressCopy(CompressionContext *restrict context, CompressionMode mode, u16 copyOffset) {
	const u16 size = context->uncompressedSize;
	size_t *restrict offset = &context->allocation->offset;
	const size_t startOffset = *offset;
	Header *header;
	chooseHeader(context, &header, size);
	/*
	* Chunk structure:
	* [ Header ][ Copy offset ][ END ]
	*/
	const size_t compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, mode, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = copyOffset;
	buffer[startOffset + compressedSize - 1] = END;
	*offset += compressedSize;
	return compressedSize;
}