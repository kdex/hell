#include "decompress.h"
#include "api.h"
#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define BUFFER(...) __VA_ARGS__
#define TEST(NAME, COMPRESSED, EXPECTED)\
	START_TEST(NAME) {\
	const u8 decompressedExpected[] = { EXPECTED };\
	u8 compressed[] = { COMPRESSED };\
	compare(compressed, decompressedExpected, sizeof decompressedExpected);\
}\
END_TEST
void compare(const u8 *compressed, const u8 *decompressedExpected, size_t decompressedSizeExpected) {
	u8 *decompressedActual = NULL;
	const size_t size = HAL.decompress(compressed, &decompressedActual);
	if (size != decompressedSizeExpected) {
		ck_abort_msg("Size mismatch. Expected %i, got %i", decompressedSizeExpected, size);
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
			ck_abort_msg("\n%s", message);
		}
		free(message);
	}
	free(decompressedActual);
}
TEST(FillByteSmall, BUFFER(
	46, 1, END
), BUFFER(
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1
))
TEST(FillByteLarge, BUFFER(
	228, 32, 1, END
), BUFFER(
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1
))
TEST(FillBytesSmall, BUFFER(
	70, 1, 2, END
), BUFFER(
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2
))
TEST(FillBytesSmallMax, BUFFER(
	95, 1, 2, END
), BUFFER(
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2
))
TEST(FillBytesLarge, BUFFER(
	232, 32, 1, 2, END
), BUFFER(
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2, 1, 2
))
TEST(FillIncrementalSequenceSmall, BUFFER(
	110, 1, END
), BUFFER(
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	11, 12, 13, 14, 15
))
START_TEST(FillIncrementalSequence_0_255) {
	u8 sequence[UINT8_MAX + 1];
	for (size_t i = 0; i <= UINT8_MAX; ++i) {
		sequence[i] = i;
	}
	u8 compressed[] = {
		236, 255, 0, END
	};
	compare(compressed, sequence, sizeof sequence);
}
END_TEST
START_TEST(FillIncrementalSequence_0_255_0_255) {
	const size_t elements = 2 * (UINT8_MAX + 1);
	u8 sequence[elements];
	for (size_t i = 0; i < elements; ++i) {
		sequence[i] = i;
	}
	u8 compressed[] = {
		236, 255, 0,
		236, 255, 0,
		END
	};
	compare(compressed, sequence, sizeof sequence);
}
END_TEST
TEST(CopyBytes, BUFFER(
	0, 1, 36, 3, 0, 7,
	134, 0, 0,
	134, 0, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	1, 3, 3, 3, 3, 3, 7,
	1, 3, 3, 3, 3, 3, 7
))
TEST(CopyBytesReversed, BUFFER(
	0, 1, 36, 3, 0, 7,
	198, 6, 0,
	198, 6, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	7, 3, 3, 3, 3, 3, 1,
	7, 3, 3, 3, 3, 3, 1
))
TEST(CopyBytesBitReversed, BUFFER(
	0, 1, 36, 3, 0, 7,
	166, 0, 0,
	166, 0, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	128, 192, 192, 192, 192, 192, 224,
	128, 192, 192, 192, 192, 192, 224
))
TEST(CopyBytesBitReversedAdvanced, BUFFER(
	0, 1, 36, 3, 0, 7,
	166, 0, 0,
	166, 0, 0,
	148, 0, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	128, 192, 192, 192, 192, 192, 224,
	128, 192, 192, 192, 192, 192, 224,
	1, 3, 3, 3, 3, 3, 7,
	128, 192, 192, 192, 192, 192, 224,
	128, 192, 192, 192, 192, 192, 224
))
START_TEST(UncompressedUint8) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 uncompressed[] = { i };
		const u8 compressed[] = { 0, i, END };
		compare(compressed, uncompressed, sizeof uncompressed);
	}
}
END_TEST
START_TEST(UncompressedUint16) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		for (u16 j = 0; j <= UINT8_MAX; ++j) {
			const u8 uncompressed[] = { i, j };
			const u8 compressed[] = { 1, i, j, END };
			compare(compressed, uncompressed, sizeof uncompressed);
		}
	}
}
END_TEST
int main(void) {
	Suite *suite = suite_create("Core");
	TCase *regular = tcase_create("Regular");
	SRunner *sr = srunner_create(suite);
	suite_add_tcase(suite, regular);
	tcase_add_test(regular, FillByteSmall);
	tcase_add_test(regular, FillByteLarge);
	tcase_add_test(regular, FillBytesSmall);
	tcase_add_test(regular, FillBytesSmallMax);
	tcase_add_test(regular, FillBytesLarge);
	tcase_add_test(regular, FillIncrementalSequenceSmall);
	tcase_add_test(regular, FillIncrementalSequence_0_255);
	tcase_add_test(regular, FillIncrementalSequence_0_255_0_255);
	tcase_add_test(regular, CopyBytes);
	tcase_add_test(regular, CopyBytesReversed);
	tcase_add_test(regular, CopyBytesBitReversed);
	tcase_add_test(regular, CopyBytesBitReversedAdvanced);
	tcase_add_test(regular, UncompressedUint8);
	tcase_add_test(regular, UncompressedUint16);
	srunner_run_all(sr, CK_ENV);
	size_t nf = srunner_ntests_failed(sr);
	srunner_free(sr);
	return nf == 0 ? 0 : 1;
}