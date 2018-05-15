#include "test-tools.h"
#include "compress.h"
#include "compression-mode.h"
#include "decompress.h"
#include "types.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
int fail(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	return EXIT_FAILURE;
}
int test(size_t (*fn)(const u8 *restrict payload, size_t payloadSize, u8 **restrict destination), const u8 *restrict payload, size_t payloadSize, const u8 *restrict expected, size_t expectedSize) {
	u8 *buffer;
	const size_t actualSize = fn(payload, payloadSize, &buffer);
	const int result = compare(buffer, actualSize, expected, expectedSize);
	free(buffer);
	return result;
}
int validateCompressionResult(
	CompressionContext *restrict context,
	size_t compressedSize,
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	size_t expectedSize
) {
	int result = EXIT_FAILURE;
	if (compressedSize) {
		/*
		* Compressors write straight into the buffer, hence we should resize the buffer to its actual size here.
		* Usually, this is done at the end of `compress`.
		*/
		resize(context->allocation, compressedSize);
		result = compare(context->allocation->buffer, compressedSize, expected, expectedSize);
	}
	if (result == EXIT_FAILURE) {
		return result;
	}
	/*
	* Lastly, we decompress the compressed data and compare it against the uncompressed payload.
	* This is a sanity check for `decompress`.
	*/
	if (uncompressed) {
		u8 *decompressed;
		const size_t decompressedSize = decompress(context->allocation->buffer, compressedSize, &decompressed);
		result = compare(decompressed, decompressedSize, uncompressed, uncompressedSize);
		free(decompressed);
	}
	return result;
}
int testUncompressed(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize
) {
	CompressionContext *restrict context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t compressedSize = compressUncompressed(context, uncompressedSize, uncompressed);
	const int result = validateCompressionResult(context, compressedSize, uncompressed, uncompressedSize, expected, expectedSize);
	freeCompressionContext(context);
	return result;
}
int testFillByte(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 byte
) {
	CompressionContext *restrict context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t compressedSize = compressFillByte(context, uncompressedSize, byte);
	const int result = validateCompressionResult(context, compressedSize, uncompressed, uncompressedSize, expected, expectedSize);
	freeCompressionContext(context);
	return result;
}
int testFillBytes(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 byteA,
	u8 byteB
) {
	CompressionContext *restrict context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t compressedSize = compressFillBytes(context, uncompressedSize, byteA, byteB);
	const int result = validateCompressionResult(context, compressedSize, uncompressed, uncompressedSize, expected, expectedSize);
	freeCompressionContext(context);
	return result;
}
int testFillIncrementalSequence(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 seed
) {
	CompressionContext *restrict context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t compressedSize = compressFillIncrementalSequence(context, uncompressedSize, seed);
	const int result = validateCompressionResult(context, compressedSize, uncompressed, uncompressedSize, expected, expectedSize);
	freeCompressionContext(context);
	return result;
}
int testCopy(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	CompressionMode mode,
	u16 copyOffset
) {
	CompressionContext *restrict context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t compressedSize = compressCopy(context, uncompressedSize, mode, copyOffset);
	const int result = validateCompressionResult(context, compressedSize, uncompressed, uncompressedSize, expected, expectedSize);
	freeCompressionContext(context);
	return result;
}
int compare(
	const u8 *restrict actual,
	size_t actualSize,
	const u8 *restrict expected,
	size_t expectedSize
) {
	if (actualSize != expectedSize) {
		return fail("Size mismatch. Expected %i, got %i\n", expectedSize, actualSize);
	}
	if (memcmp(expected, actual, actualSize)) {
		char *message = calloc(1, 1);
		size_t messageSize = 1;
		const char *offsetTitle = "Offset";
		const char *expectedTitle = "Expected value";
		const char *actualTitle = "Actual value";
		const char *attentionTitle = "!";
		const size_t padding = 3;
		const size_t offsetWidth = strlen(offsetTitle) + padding;
		const size_t expectedWidth = strlen(expectedTitle) + padding;
		const size_t actualWidth = strlen(actualTitle) + padding;
		const size_t attentionWidth = strlen(attentionTitle) + padding;
		/* Append heading to message */
		const char *headingFormat = "%*s%*s%*s%*s\n";
		const size_t headingSize = 1 + snprintf(
			NULL,
			0,
			headingFormat,
			offsetWidth,
			offsetTitle,
			expectedWidth,
			expectedTitle,
			actualWidth,
			actualTitle,
			attentionWidth,
			attentionTitle
		);
		messageSize += headingSize - 1;
		message = realloc(message, messageSize);
		char *heading = malloc(headingSize);
		snprintf(
			heading,
			headingSize,
			headingFormat,
			offsetWidth,
			offsetTitle,
			expectedWidth,
			expectedTitle,
			actualWidth,
			actualTitle,
			attentionWidth,
			attentionTitle
		);
		strncat(message, heading, headingSize);
		free(heading);
		/* Append (mis)matches to message */
		bool mismatch = false;
		int maxLines = 10;
		for (size_t i = 0; i < actualSize; ++i) {
			const bool hasMismatch = expected[i] != actual[i];
			if (hasMismatch) {
				mismatch = true;
			}
			if (mismatch && maxLines-- > 0) {
				const char *comparisonFormat = "%*lu%*lu%*lu%*s\n";
				const char *notice = hasMismatch ? "<--" : "";
				const size_t comparisonSize = 1 + snprintf(
					NULL,
					0,
					comparisonFormat,
					offsetWidth,
					i,
					expectedWidth,
					expected[i],
					actualWidth,
					actual[i],
					attentionWidth,
					notice
				);
				messageSize += comparisonSize - 1;
				message = realloc(message, messageSize);
				char *comparison = malloc(comparisonSize);
				snprintf(
					comparison,
					comparisonSize,
					comparisonFormat,
					offsetWidth,
					i,
					expectedWidth,
					expected[i],
					actualWidth,
					actual[i],
					attentionWidth,
					notice
				);
				strncat(message, comparison, comparisonSize);
				free(comparison);
			}
		}
		if (mismatch) {
			const int value = fail("\n%s", message);
			free(message);
			return value;
		}
		free(message);
	}
	return EXIT_SUCCESS;
}