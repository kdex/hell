#include "test-tools.h"
#include "compress.h"
#include "decompress.h"
#include "types.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
int fail(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	return EXIT_FAILURE;
}
int test(size_t (*fn)(const u8 *payload, size_t payloadSize, u8 **destination), const u8 *payload, size_t payloadSize, const u8 *expected, size_t expectedSize) {
	u8 *buffer;
	const size_t actualSize = fn(payload, payloadSize, &buffer);
	const int result = compare(buffer, expected, actualSize, expectedSize);
	free(buffer);
	return result;
}
int testUncompressedU8(const u8 *payload, size_t payloadSize, const u8 *expected, size_t expectedSize) {
	CompressionContext *context = malloc(sizeof *context);
	initCompressionContext(context);
	const size_t actualSize = compressUncompressedU8(context, payload, payloadSize);
	if (actualSize) {
		/*
		* Compressors write straight into the buffer, hence should resize the buffer to its actual size here.
		* Usually, this is done at the end of `compress`.
		*/
		resize(context->allocation, actualSize);
		const int result = compare(context->allocation->buffer, expected, actualSize, expectedSize);
		freeCompressionContext(context);
		return result;
	}
	freeCompressionContext(context);
	return EXIT_FAILURE;
}
int compare(const u8 *actual, const u8 *expected, size_t actualSize, size_t expectedSize) {
	if (actualSize != expectedSize) {
		return fail("Size mismatch. Expected %i, got %i", expectedSize, actualSize);
	}
	if (memcmp(expected, actual, actualSize)) {
		char *message = malloc(1);
		size_t messageSize = 1;
		message[0] = '\0';
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