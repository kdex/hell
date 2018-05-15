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
	const size_t maxWindowSize = context->large->capacity;
	size_t compressedSize = 0;
	const size_t maxIndex = uncompressedSize - 1;
	/* Index of where the window should start. Indicates first position of yet uncompressed data */
	size_t searchStart = 0;
	/* Index of where the window should end. Indicates last position of yet uncompressed data */
	size_t windowEnd = min(maxWindowSize - 1, maxIndex);
	/* TODO: Implement */
	size_t lookAheadStart = 0;
	size_t uncompressedBuffer = 0;
	while (lookAheadStart < uncompressedSize) {
		bool lzFailed = false;
		const size_t windowSize = windowEnd - searchStart + 1;
		if (searchStart < lookAheadStart) {
			/* Dictionary exists */
			const size_t availableLookAhead = windowEnd - lookAheadStart + 1;
			size_t i = lookAheadStart - 1;
			size_t bestOffset;
			u16 matchLength = 0;
			do {
				const bool isMatch = uncompressed[i] == uncompressed[lookAheadStart];
				if (isMatch) {
					u16 length = 1;
					for (size_t j = 1; j < min(availableLookAhead, lookAheadStart - i); ++j) {
						if (uncompressed[i + j] == uncompressed[lookAheadStart + j]) {
							++length;
						}
					}
					if (matchLength < length) {
						bestOffset = lookAheadStart - i;
						matchLength = length;
					}
				}
			}
			while (i-- != searchStart);
			if (matchLength) {
				/* Matches found */
				compressedSize += compressCopy(context, matchLength, COPY_BYTES, bestOffset);
				lookAheadStart += matchLength;
// 				searchStart = min(searchStart + matchLength, maxIndex);
				windowEnd = min(windowEnd + matchLength, maxIndex);
				
			}
			else {
				lzFailed = true;
			}
			/* TODO: Write out uncompressed buffer if possible? */
		}
		else {
			lzFailed = true;
		}
		if (lzFailed) {
			const bool checkPairs = searchStart + 2 <= maxIndex;
			const u8 byteA = uncompressed[searchStart];
			u8 byteB;
			u16 pairedBytes = 1;
			u16 matched = 1 + uncompressedBuffer;
			CompressionMode leader = UNCOMPRESSED;
			{
				if (checkPairs) {
					byteB = uncompressed[searchStart + 1];
					for (size_t i = searchStart + 2; i < searchStart + windowSize - 1; i += 2) {
						if (uncompressed[i] != byteA || uncompressed[i + 1] != byteB) {
							break;
						}
						++pairedBytes;
					}
					if (matched < pairedBytes) {
						matched = 2 * pairedBytes;
						leader = FILL_BYTES;
					}
				}
			}
			{
				/* Check for FILL_BYTE */
				u16 constantBytes = 0;
				for (size_t i = searchStart; i < searchStart + windowSize; ++i) {
					if (uncompressed[i] != byteA) {
						break;
					}
					++constantBytes;
				}
				if (matched < constantBytes) {
					matched = constantBytes;
					leader = FILL_BYTE;
				}
			}
			{
				u16 incrementalBytes = 0;
				/* Check for FILL_INCREMENTAL_SEQUENCE */
				for (size_t i = 0; i < searchStart + windowSize; ++i) {
					/* TODO: Cast necessary? */
					if (uncompressed[i] != (u8) (byteA + i)) {
						break;
					}
					++incrementalBytes;
				}
				if (matched < incrementalBytes) {
					matched = incrementalBytes;
					leader = FILL_INCREMENTAL_SEQUENCE;
				}
			}
			if (leader != UNCOMPRESSED && uncompressedBuffer) {
				compressedSize += compressUncompressed(context, uncompressedBuffer, uncompressed + searchStart);
				uncompressedBuffer = 0;
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
					++uncompressedBuffer;
					break;
			}
// 			searchStart += min(searchStart + constantBytes, maxIndex);
// 			windowEnd = min(windowEnd + constantBytes, maxIndex);
			lookAheadStart += matched;
		}
	}
	if (uncompressedBuffer) {
		compressedSize += compressUncompressed(context, uncompressedBuffer, uncompressed + searchStart);
	}
	*compressed = malloc(compressedSize);
	if (compressedSize) {
		memcpy(*compressed, context->allocation->buffer, compressedSize);
	}
	freeCompressionContext(context);
	return compressedSize;
}