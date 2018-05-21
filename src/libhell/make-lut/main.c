#include "common/constants.h"
#include "common/types.h"
#include "io/io.h"
#include <stdio.h>
#include <stdint.h>
int main(int argc, char *argv[]) {
	const char *invocation = argv[0];
	if (argc == 2) {
		const char *path = argv[1];
		FILE *fp = fopen(path, "w");
		if (fp) {
			fprintf(fp, "#include \"make-lut/lut.h\"\n");
			fprintf(fp, "#include \"common/types.h\"\n");
			fprintf(fp, "const u8 reverses[] = { ");
			/* This is an internal optimization for the decompression method. Every element in this array is the binary reverse of its index. Don't confuse "reverse" with "negation". */
			for (u16 n = 0; n <= UINT8_MAX; ++n) {
				u8 reverse = 0;
				for (u8 i = 0; i < BITS_IN_BYTE; ++i) {
					if (n & (1 << i)) {
						reverse |= 1 << (BITS_IN_BYTE - 1 - i);
					}
				}
				fprintf(fp, "%d, ", reverse);
			}
			fprintf(fp, "};");
		}
		else {
			failWithFileError(path, invocation);
		}
	}
	else {
		printf("Syntax: %s <OUTPUT_PATH>\n", invocation);
	}
}