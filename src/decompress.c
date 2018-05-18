#include "decompress.h"
#include "allocation.h"
#include "constants.h"
#include "compression-mode.h"
#include "lut.h"
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
size_t decompress(const u8 *restrict compressed, size_t compressedSize, u8 **restrict decompressed) {
	Allocation *restrict allocation = malloc(sizeof *allocation);
	initAllocation(allocation);
	/* TODO: Figure out the maximum sizes */
	size_t bytesRead = 0;
	size_t bytesWritten = 0;
	while (bytesRead < compressedSize) {
		const u8 header = compressed[bytesRead++];
		if (header == END) {
			break;
		}
		/* `header` contains both mode and length information. Split it up. */
		u8 mode = header >> 5;
		size_t sourceLength = 0;
		if (mode != EXTEND_HEADER) {
			/* The last five bits contain information about how long the source data will be */
			sourceLength = 1 + (header & 0x1f);
		}
		else {
			mode = (header & 0x1c) >> 2;
			if (mode != EXTEND_HEADER) {
				sourceLength = ((header & 3) << 8) + compressed[bytesRead++] + 1;
			}
		}
		u16 copyOffset = 0;
		if (mode >= COPY_BYTES && mode < EXTEND_HEADER) {
			const u8 msb = compressed[bytesRead++];
			const u8 lsb = compressed[bytesRead++];
			copyOffset = msb << 8 | lsb;
		}
		reserve(allocation, bytesWritten + sourceLength);
		switch (mode) {
			case UNCOMPRESSED: {
				memcpy(allocation->buffer + bytesWritten, compressed + bytesRead, sourceLength);
				bytesRead += sourceLength;
				bytesWritten += sourceLength;
				break;
			}
			case FILL_BYTE: {
				memset(allocation->buffer + bytesWritten, compressed[bytesRead++], sourceLength);
				bytesWritten += sourceLength;
				break;
			}
			case FILL_BYTES: {
				const u8 left = compressed[bytesRead++];
				const u8 right = compressed[bytesRead++];
				for (size_t k = 0; k < sourceLength; ++k) {
					allocation->buffer[bytesWritten++] = left;
					allocation->buffer[bytesWritten++] = right;
				}
				break;
			}
			case FILL_INCREMENTAL_SEQUENCE: {
				const u8 seed = compressed[bytesRead++];
				for (size_t k = 0; k < sourceLength; ++k) {
					allocation->buffer[bytesWritten++] = seed + k;
				}
				break;
			}
			case COPY_BYTES: {
				/* TODO: memmove? */
				memcpy(allocation->buffer + bytesWritten, allocation->buffer + copyOffset, sourceLength);
				bytesWritten += sourceLength;
				break;
			}
			case COPY_REVERSED_BITS: {
				for (size_t k = 0; k < sourceLength; ++k) {
					allocation->buffer[bytesWritten++] = reverses[allocation->buffer[copyOffset + k]];
				}
				break;
			}
			case COPY_REVERSED_BYTES: {
				for (size_t k = 0; k < sourceLength; ++k) {
					allocation->buffer[bytesWritten++] = allocation->buffer[copyOffset - k];
				}
				break;
			}
		}
	}
	assert(bytesRead == compressedSize);
	if (allocation->size > bytesWritten) {
		resize(allocation, bytesWritten);
	}
	size_t size = allocation->size;
	if (size && bytesRead == compressedSize) {
		*decompressed = malloc(size);
		memcpy(*decompressed, allocation->buffer, size);
	}
	else {
		*decompressed = NULL;
	}
	freeAllocation(allocation);
	return size;
}