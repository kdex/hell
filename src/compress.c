#include "compress.h"
#include "constants.h"
#include "compression-context.h"
#include "compressors.h"
#include "types.h"
#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
size_t compress(const u8 *restrict uncompressed, size_t uncompressedSize, u8 **restrict compressed) {
	if (uncompressedSize > MAX_PAYLOAD) {
		*compressed = NULL;
		return 0;
	}
	CompressionContext *context = malloc(sizeof *context);
	initCompressionContext(context);
	size_t position = 0;
	while (position < uncompressedSize) {
		bool lzFailed = false;
		if (position) {
			/* Dictionary exists */
			size_t bestOffset;
			u16 matchLength = 0;
			for (size_t i = 0; i < position; ++i) {
				const bool isMatch = uncompressed[i] == uncompressed[position];
				if (isMatch) {
					u16 length = 1;
					const size_t searchSpace = min(
						uncompressedSize - position,
						context->large->capacity
					);
					for (size_t j = 1; j < searchSpace; ++j) {
						if (uncompressed[i + j % (position - i)] != uncompressed[position + j]) {
							break;
						}
						++length;
					}
					if (matchLength < length) {
						bestOffset = i;
						matchLength = length;
					}
					assert(length <= context->large->capacity);
				}
			}
			if (matchLength > 3) {
				/* Matches found */
				flushStash(context, uncompressed);
				compressCopy(context, matchLength, COPY_BYTES, bestOffset);
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
			const bool checkPairs = position + 3 < uncompressedSize;
			const u8 byteA = uncompressed[position];
			u8 byteB;
			u16 matched = 1;
			CompressionMode leader = UNCOMPRESSED;
			{
				if (checkPairs) {
					u16 pairs = 1;
					byteB = uncompressed[position + 1];
					for (size_t i = position + 2; i < uncompressedSize - 1; i += 2) {
						if (uncompressed[i] != byteA || uncompressed[i + 1] != byteB) {
							break;
						}
						++pairs;
					}
					const u16 matches = 2 * pairs;
					if (matches > 2 && matched < matches) {
						matched = matches;
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
				if (matches > 2 && matched < matches) {
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
				if (matches > 2 && matched < matches) {
					matched = matches;
					leader = FILL_INCREMENTAL_SEQUENCE;
				}
			}
			if (leader != UNCOMPRESSED) {
				flushStash(context, uncompressed);
			}
			switch (leader) {
				case FILL_BYTE:
					compressFillByte(context, matched, byteA);
					break;
				case FILL_BYTES:
					compressFillBytes(context, matched / 2, byteA, byteB);
					break;
				case FILL_INCREMENTAL_SEQUENCE:
					compressFillIncrementalSequence(context, matched, byteA);
					break;
				default:
					stash(context, position);
					break;
			}
			position += matched;
		}
	}
	flushStash(context, uncompressed);
	terminateCompressionContext(context);
	const size_t compressedSize = context->allocation->written;
	*compressed = malloc(compressedSize);
	if (compressedSize) {
		memcpy(*compressed, context->allocation->buffer, compressedSize);
	}
	freeCompressionContext(context);
	return compressedSize;
}