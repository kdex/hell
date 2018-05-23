#include "core/compressors.h"
#include "common/restrict.h"
#include "common/types.h"
#include "core/compression-context.h"
#include "core/compression-mode.h"
#include "core/header.h"
#include <stdlib.h>
void chooseHeader(const CompressionContext *RESTRICT context, u16 size, Header **RESTRICT header) {
	if (size <= context->small->capacity) {
		*header = context->small;
	}
	else {
		*header = context->large;
	}
}
u16 compressUncompressed(CompressionContext *RESTRICT context, u16 size, const u8 *RESTRICT payload) {
	size_t *RESTRICT written = &context->allocation->written;
	const size_t offset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Uncompressed data ]
	*/
	const u16 compressedSize = header->size + size;
	reserve(context->allocation, compressedSize);
	u8 *RESTRICT buffer = context->allocation->buffer;
	buffer[offset] = makeFirstByte(header, UNCOMPRESSED, size);
	buffer[offset + 1] = size - 1;
	for (u16 i = 0; i < size; ++i) {
		buffer[offset + header->size + i] = payload[i];
	}
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillByte(CompressionContext *RESTRICT context, u16 size, u8 byte) {
	size_t *RESTRICT written = &context->allocation->written;
	const size_t offset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Byte value ]
	*/
	const u16 compressedSize = header->size + 1;
	reserve(context->allocation, compressedSize);
	u8 *RESTRICT buffer = context->allocation->buffer;
	buffer[offset] = makeFirstByte(header, FILL_BYTE, size);
	buffer[offset + 1] = size - 1;
	buffer[offset + header->size] = byte;
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillBytes(CompressionContext *RESTRICT context, u16 size, u8 byteA, u8 byteB) {
	size_t *RESTRICT written = &context->allocation->written;
	const size_t offset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Byte A ][ Byte B ]
	*/
	const u16 compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *RESTRICT buffer = context->allocation->buffer;
	buffer[offset] = makeFirstByte(header, FILL_BYTES, size);
	buffer[offset + 1] = size - 1;
	buffer[offset + header->size] = byteA;
	buffer[offset + header->size + 1] = byteB;
	*written += compressedSize;
	return compressedSize;
}
u16 compressFillIncrementalSequence(CompressionContext *RESTRICT context, u16 size, u8 seed) {
	size_t *RESTRICT written = &context->allocation->written;
	const size_t offset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Seed ]
	*/
	const u16 compressedSize = header->size + 1;
	reserve(context->allocation, compressedSize);
	u8 *RESTRICT buffer = context->allocation->buffer;
	buffer[offset] = makeFirstByte(header, FILL_INCREMENTAL_SEQUENCE, size);
	buffer[offset + 1] = size - 1;
	buffer[offset + header->size] = seed;
	*written += compressedSize;
	return compressedSize;
}
u16 compressCopy(CompressionContext *RESTRICT context, u16 size, CompressionMode mode, u16 copyOffset) {
	size_t *RESTRICT written = &context->allocation->written;
	const size_t offset = *written;
	Header *header;
	chooseHeader(context, size, &header);
	/*
	* Chunk structure:
	* [ Header ][ Copy offset MSB ][ Copy offset LSB ]
	*/
	const u16 compressedSize = header->size + 2;
	reserve(context->allocation, compressedSize);
	u8 *RESTRICT buffer = context->allocation->buffer;
	buffer[offset] = makeFirstByte(header, mode, size);
	buffer[offset + 1] = size - 1;
	buffer[offset + header->size] = (copyOffset & 0xff00) >> 8;
	buffer[offset + header->size + 1] = (u8) copyOffset;
	*written += compressedSize;
	return compressedSize;
}