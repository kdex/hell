#include "test-tools.h"
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
int compare(const u8 *compressed, const u8 *decompressedExpected, size_t decompressedSizeExpected) {
	u8 *decompressedActual = NULL;
	const size_t size = decompress(compressed, &decompressedActual);
	if (size != decompressedSizeExpected) {
		return fail("Size mismatch. Expected %i, got %i", decompressedSizeExpected, size);
	}
	if (memcmp(decompressedExpected, decompressedActual, size)) {
		char *message = malloc(1);
		size_t messageSize = 1;
		message[0] = '\0';
		const char *offset = "Offset";
		const char *expected = "Expected value";
		const char *actual = "Actual value";
		const char *attention = "!";
		const size_t padding = 3;
		const size_t offsetWidth = strlen(offset) + padding;
		const size_t expectedWidth = strlen(expected) + padding;
		const size_t actualWidth = strlen(actual) + padding;
		const size_t attentionWidth = strlen(attention) + padding;
		/* Append heading to message */
		const char *headingFormat = "%*s%*s%*s%*s\n";
		const size_t headingSize = 1 + snprintf(
			NULL,
			0,
			headingFormat,
			offsetWidth,
			offset,
			expectedWidth,
			expected,
			actualWidth,
			actual,
			attentionWidth,
			attention
		);
		messageSize += headingSize - 1;
		message = realloc(message, messageSize);
		char *heading = malloc(headingSize);
		snprintf(
			heading,
			headingSize,
			headingFormat,
			offsetWidth,
			offset,
			expectedWidth,
			expected,
			actualWidth,
			actual,
			attentionWidth,
			attention
		);
		strncat(message, heading, headingSize);
		free(heading);
		/* Append (mis)matches to message */
		bool mismatch = false;
		int maxLines = 10;
		for (size_t i = 0; i < size; ++i) {
			const bool hasMismatch = decompressedExpected[i] != decompressedActual[i];
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
					decompressedExpected[i],
					actualWidth,
					decompressedActual[i],
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
					decompressedExpected[i],
					actualWidth,
					decompressedActual[i],
					attentionWidth,
					notice
				);
				strncat(message, comparison, comparisonSize);
			}
		}
		if (mismatch) {
			return fail("\n%s", message);
		}
		free(message);
	}
	free(decompressedActual);
	return EXIT_SUCCESS;
}