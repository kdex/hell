#include "compress.h"
#include "compressors.h"
#include "compression-context.h"
#include "types.h"
#include "util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
size_t compress(const u8 *restrict uncompressed, size_t uncompressedSize, u8 **restrict compressed) {
	CompressionContext *context = malloc(sizeof *context);
	initCompressionContext(context);
	size_t compressedSize = 0;
	const size_t maxIndex = uncompressedSize - 1;
	size_t position = 0;
	size_t bufferSize = 0;
	size_t bufferOffset = 0;
	while (position < uncompressedSize) {
		bool lzFailed = false;
		if (position) {
			/* Dictionary exists */
			const size_t availableLookAhead = maxIndex - position + 1;
			size_t i = position - 1;
			size_t bestOffset;
			u16 matchLength = 0;
			/* TODO: Reverse this search to return early */
			do {
				const bool isMatch = uncompressed[i] == uncompressed[position];
				if (isMatch) {
					u16 length = 1;
					for (size_t j = 1; j < min(availableLookAhead, position - i); ++j) {
						if (uncompressed[i + j] == uncompressed[position + j]) {
							++length;
						}
					}
					if (matchLength < length) {
						bestOffset = i;
						matchLength = length;
					}
				}
			}
			while (i--);
			if (matchLength) {
				if (bufferSize) {
					compressedSize += compressUncompressed(context, bufferSize, uncompressed + bufferOffset);
					bufferSize = 0;
				}
				/* Matches found */
				compressedSize += compressCopy(context, matchLength, COPY_BYTES, bestOffset);
				position += matchLength;
			}
			else {
				lzFailed = true;
			}
		}
		else {
			lzFailed = true;
		}
		if (lzFailed) {
			const bool checkPairs = position + 3 <= maxIndex;
			const u8 byteA = uncompressed[position];
			u8 byteB;
			u16 matched = 1;
			CompressionMode leader = UNCOMPRESSED;
			{
				if (checkPairs) {
					u16 matches = 1;
					byteB = uncompressed[position + 1];
					for (size_t i = position + 2; i < uncompressedSize - 1; i += 2) {
						if (uncompressed[i] != byteA || uncompressed[i + 1] != byteB) {
							break;
						}
						++matches;
					}
					if (matched < matches) {
						matched = 2 * matches;
						leader = FILL_BYTES;
					}
				}
			}
			{
				u16 matches = 0;
				for (size_t i = position; i < uncompressedSize; ++i) {
					if (uncompressed[i] != byteA) {
						break;
					}
					++matches;
				}
				if (matched < matches) {
					matched = matches;
					leader = FILL_BYTE;
				}
			}
			{
				u16 matches = 0;
				for (size_t i = 0; i < uncompressedSize - position; ++i) {
					if (uncompressed[position + i] != (u8) (byteA + i)) {
						break;
					}
					++matches;
				}
				if (matched < matches) {
					matched = matches;
					leader = FILL_INCREMENTAL_SEQUENCE;
				}
			}
			if (leader != UNCOMPRESSED && bufferSize) {
				compressedSize += compressUncompressed(context, bufferSize, uncompressed + bufferOffset);
				bufferSize = 0;
			}
			switch (leader) {
				case FILL_BYTE:
					compressedSize += compressFillByte(context, matched, byteA);
					break;
				case FILL_BYTES:
					compressedSize += compressFillBytes(context, matched, byteA, byteB);
					break;
				case FILL_INCREMENTAL_SEQUENCE:
					compressedSize += compressFillIncrementalSequence(context, matched, byteA);
					break;
				default:
					if (!bufferSize) {
						bufferOffset = position;
					}
					++bufferSize;
					break;
			}
			position += matched;
		}
	}
	if (bufferSize) {
		compressedSize += compressUncompressed(context, bufferSize, uncompressed + bufferOffset);
	}
	compressedSize += terminateCompressionContext(context);
	*compressed = malloc(compressedSize);
	if (compressedSize) {
		memcpy(*compressed, context->allocation->buffer, compressedSize);
	}
	freeCompressionContext(context);
	return compressedSize;
}