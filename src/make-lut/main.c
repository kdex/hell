#include "types.h"
#include <stdio.h>
#include <stdint.h>
int main(int argc, char *argv[]) {
	const u8 WIDTH = 8;
	if (argc == 2) {
		const char *path = argv[1];
		FILE *fp = fopen(path, "w");
		fprintf(fp, "#include \"lut.h\"\n");
		fprintf(fp, "#include \"types.h\"\n");
		fprintf(fp, "const u8 reverses[] = { ");
		/* This is an internal optimization for the decompression method. Every element in this array is the binary reverse of its index. Don't confuse "reverse" with "negation". */
		for (u16 n = 0; n <= UINT8_MAX; ++n) {
			u8 reverse = 0;
			for (u8 i = 0; i < WIDTH; ++i) {
				if (n & (1 << i)) {
					reverse |= 1 << (WIDTH - 1 - i);
				}
			}
			fprintf(fp, "%d, ", reverse);
		}
		fprintf(fp, "};");
	}
	else {
		printf("Syntax: %s <OUTPUT_PATH>\n", argv[0]);
	}
}