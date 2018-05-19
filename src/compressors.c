#include "compressors.h"
#include "compression-mode.h"
#include "compression-context.h"
#include "header.h"
#include "types.h"
#include <stdlib.h>
void chooseHeader(const CompressionContext *restrict context, u16 size, Header **restrict header) {
	if (size <= context->small->capacity) {
		*header = context->small;
	}
	else {
		*header = context->large;
	}
}
u16 compressUncompressed(CompressionContext *restrict context, u16 size, const u8 *restrict payload) {
	size_t *restrict written = &context->allocation->written;
	const size_t startOffset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Uncompressed data ]
	*/
	const u16 compressedSize = header->size + size;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, UNCOMPRESSED, size);
	buffer[startOffset + 1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		buffer[startOffset + header->size + i] = payload[i];
	}
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillByte(CompressionContext *restrict context, u16 size, u8 byte) {
	size_t *restrict written = &context->allocation->written;
	const size_t startOffset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Byte value ]
	*/
	const u16 compressedSize = header->size + 1;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_BYTE, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = byte;
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillBytes(CompressionContext *restrict context, u16 size, u8 byteA, u8 byteB) {
	size_t *restrict written = &context->allocation->written;
	const size_t startOffset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Byte A ][ Byte B ]
	*/
	const u16 compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_BYTES, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = byteA;
	buffer[startOffset + header->size + 1] = byteB;
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillIncrementalSequence(CompressionContext *restrict context, u16 size, u8 seed) {
	size_t *restrict written = &context->allocation->written;
	const size_t startOffset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Seed ]
	*/
	const u16 compressedSize = header->size + 1;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, FILL_INCREMENTAL_SEQUENCE, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = seed;
	*written += compressedSize;
	return compressedSize;
}
u16 compressCopy(CompressionContext *restrict context, u16 size, CompressionMode mode, u16 copyOffset) {
	size_t *restrict written = &context->allocation->written;
	const size_t startOffset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Copy offset MSB ][ Copy offset LSB ]
	*/
	const u16 compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *restrict buffer = context->allocation->buffer;
	buffer[startOffset] = makeFirstByte(header, mode, size);
	buffer[startOffset + 1] = size - 1;
	buffer[startOffset + header->size] = (copyOffset & 0xff00) >> 8;
	buffer[startOffset + header->size + 1] = copyOffset;
	*written += compressedSize;
	return compressedSize;
}