#include "decompress.h"
#include "allocation.h"
#include "compression-mode.h"
#include "lut.h"
#include "types.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
size_t decompress(const u8 *compressed, size_t compressedSize, u8 **decompressed) {
	Allocation *allocation = malloc(sizeof *allocation);
	initAllocation(allocation);
	/* TODO: Figure out the maximum sizes */
	size_t bytesRead = 0;
	size_t bytesWritten = 0;
	for (size_t i = 0; i < compressedSize; ++i) {
		u8 command = compressed[bytesRead++];
		if (command == END) {
			break;
		}
		/* `command` contains both length and type information. Split it up. */
		u8 commandType = command >> 5;
		size_t sourceLength = 0;
		if (commandType != EXTEND_COMMAND) {
			/* The last five bits contain information about how long the source data will be */
			sourceLength = 1 + (command & 0x1F);
		}
		else {
			commandType = (command & 0x1C) >> 2;
			if (commandType != EXTEND_COMMAND) {
				sourceLength = ((command & 3) << 8) + compressed[bytesRead++] + 1;
			}
		}
		u16 copyOffset = 0;
		if (commandType >= COPY_BYTES && commandType < EXTEND_COMMAND) {
			/* copyOffset is stored as u16 BE (TODO: verify) */
			const u8 low = compressed[bytesRead++];
			const u8 high = compressed[bytesRead++];
			copyOffset = high << 8 | low;
		}
		reserve(allocation, bytesWritten + sourceLength);
		switch (commandType) {
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
	if (allocation->size > bytesWritten) {
		resize(allocation, bytesWritten);
	}
	size_t size = allocation->size;
	if (size) {
		*decompressed = malloc(size);
		memcpy(*decompressed, allocation->buffer, size);
	}
	else {
		*decompressed = NULL;
	}
	freeAllocation(allocation);
	return size;
}