#include "core/decompress.h"
#include "common/constants.h"
#include "common/types.h"
#include "core/allocation.h"
#include "core/compression-mode.h"
#include "core/make-lut/lut.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
size_t decompress(const u8 *restrict compressed, size_t compressedSize, u8 **restrict decompressed) {
	Allocation *restrict allocation = malloc(sizeof *allocation);
	initAllocation(allocation);
	/* TODO: Figure out the maximum sizes */
	size_t read = 0;
	size_t *written = &allocation->written;
	while (read < compressedSize) {
		const u8 header = compressed[read++];
		if (header == END) {
			break;
		}
		/* `header` contains both mode and length information. Split it up. */
		u8 mode = header >> 5;
		u16 storedLength = 0;
		if (mode != EXTEND_HEADER) {
			/* The last five bits contain information about how long the source data will be */
			storedLength = 1 + (header & 0x1f);
		}
		else {
			mode = (header & 0x1c) >> 2;
			if (mode != EXTEND_HEADER) {
				storedLength = ((header & 3) << 8) + compressed[read++] + 1;
			}
		}
		const u16 sourceLength = storedLength * (mode == FILL_BYTES ? 2 : 1);
		u16 copyOffset = 0;
		if (mode >= COPY_BYTES && mode < EXTEND_HEADER) {
			const u8 msb = compressed[read++];
			const u8 lsb = compressed[read++];
			copyOffset = msb << 8 | lsb;
		}
		reserve(allocation, sourceLength);
		switch (mode) {
			case UNCOMPRESSED: {
				memcpy(allocation->buffer + *written, compressed + read, storedLength);
				read += storedLength;
				*written += storedLength;
				break;
			}
			case FILL_BYTE: {
				memset(allocation->buffer + *written, compressed[read++], storedLength);
				*written += storedLength;
				break;
			}
			case FILL_BYTES: {
				const u8 left = compressed[read++];
				const u8 right = compressed[read++];
				for (size_t i = 0; i < storedLength; ++i) {
					allocation->buffer[(*written)++] = left;
					allocation->buffer[(*written)++] = right;
				}
				break;
			}
			case FILL_INCREMENTAL_SEQUENCE: {
				const u8 seed = compressed[read++];
				for (size_t i = 0; i < storedLength; ++i) {
					allocation->buffer[(*written)++] = seed + i;
				}
				break;
			}
			case COPY_BYTES: {
				for (size_t i = 0; i < storedLength; ++i) {
					allocation->buffer[(*written)++] = allocation->buffer[copyOffset + i];
				}
				break;
			}
			case COPY_REVERSED_BITS: {
				for (size_t i = 0; i < storedLength; ++i) {
					allocation->buffer[(*written)++] = reverses[allocation->buffer[copyOffset + i]];
				}
				break;
			}
			case COPY_REVERSED_BYTES: {
				for (size_t i = 0; i < storedLength; ++i) {
					allocation->buffer[(*written)++] = allocation->buffer[copyOffset - i];
				}
				break;
			}
		}
	}
	assert(read == compressedSize);
	const size_t size = *written;
	resize(allocation, size);
	if (size && read == compressedSize) {
		*decompressed = malloc(size);
		memcpy(*decompressed, allocation->buffer, size);
	}
	else {
		*decompressed = NULL;
	}
	freeAllocation(allocation);
	return size;
}