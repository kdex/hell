#include "core/compress.h"
#include "common/constants.h"
#include "common/types.h"
#include "core/compression-context.h"
#include "core/compression-mode.h"
#include "core/compressors.h"
#include "core/make-lut/lut.h"
#include "core/util.h"
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
		const u8 byteA = uncompressed[position];
		u8 byteB = 0;
		u16 matched = 1;
		CompressionMode leader = UNCOMPRESSED;
		{
			if (position + 3 < uncompressedSize) {
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
		size_t bestOffset = 0;
		const size_t forwardSearchSpace = min(uncompressedSize - position, context->large->capacity);
		for (size_t i = 0; i < position; ++i) {
			const bool isExactMatch = uncompressed[i] == uncompressed[position];
			const bool isBitReversedMatch = reverses[uncompressed[i]] == uncompressed[position];
			if (isExactMatch) {
				{
					u16 matches = 1;
					for (size_t j = 1; j < forwardSearchSpace; ++j) {
						if (uncompressed[i + j % (position - i)] != uncompressed[position + j]) {
							break;
						}
						++matches;
					}
					if (matched < matches) {
						bestOffset = i;
						matched = matches;
						leader = COPY_BYTES;
					}
				}
				{
					u16 matches = 1;
					const size_t backwardSearchSpace = min(i + 1, forwardSearchSpace);
					for (size_t j = 1; j < backwardSearchSpace; ++j) {
						if (uncompressed[i - j] != uncompressed[position + j]) {
							break;
						}
						++matches;
					}
					if (matched < matches) {
						bestOffset = i;
						matched = matches;
						leader = COPY_REVERSED_BYTES;
					}
				}
			}
			if (isBitReversedMatch) {
				u16 matches = 1;
				for (size_t j = 1; j < forwardSearchSpace; ++j) {
					if (reverses[uncompressed[i + j % (position - i)]] != uncompressed[position + j]) {
						break;
					}
					++matches;
				}
				if (matched < matches) {
					bestOffset = i;
					matched = matches;
					leader = COPY_REVERSED_BITS;
				}
			}
			assert(matched <= context->large->capacity);
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
			case COPY_BYTES:
			case COPY_REVERSED_BITS:
			case COPY_REVERSED_BYTES:
				compressCopy(context, matched, leader, bestOffset);
				break;
			default:
				stash(context, position);
				break;
		}
		position += matched;
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