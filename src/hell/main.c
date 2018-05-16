#include "compress.h"
#include "decompress.h"
#include "io.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int printSyntax(const char *invocation) {
	fprintf(stderr, "Syntax:\n\t%s compress <INPUT> <OUTPUT>\n\t%s decompress <INPUT> <OUTPUT>\n", invocation, invocation);
	return EXIT_FAILURE;
}
int main(int argc, const char *argv[]) {
	const char *invocation = argv[0];
	if (argc < 2) {
		return printSyntax(invocation);
	}
	else {
		const char *mode = argv[1];
		if (!strcmp(mode, "compress")) {
			if (argc < 4) {
				fprintf(stderr, "Not enough arguments for mode \"%s\".\n", mode);
				return printSyntax(invocation);
			}
			u8 *inputFile;
			const char *inputFilename = argv[2];
			const char *outputFilename = argv[3];
			const size_t inputSize = readFile(inputFilename, &inputFile);
			if (!inputFile) {
				printFileError(inputFilename, invocation);
				return EXIT_FAILURE;
			}
			u8 *compressed;
			const size_t size = compress(inputFile, inputSize, &compressed);
			free(inputFile);
			if (!size) {
				fprintf(stderr, "%s doesn't seem to be a valid file.", inputFilename);
				return EXIT_FAILURE;
			}
			const bool failed = writeFile(outputFilename, compressed, size);
			free(compressed);
			if (failed) {
				printFileError(outputFilename, invocation);
				return EXIT_FAILURE;
			}
		}
		else if (!strcmp(mode, "decompress")) {
			if (argc < 4) {
				fprintf(stderr, "Not enough arguments for mode \"%s\".\n", mode);
				return printSyntax(invocation);
			}
			u8 *inputFile;
			const char *inputFilename = argv[2];
			const char *outputFilename = argv[3];
			const size_t inputSize = readFile(inputFilename, &inputFile);
			if (!inputFile) {
				printFileError(inputFilename, invocation);
				return EXIT_FAILURE;
			}
			u8 *decompressed;
			const size_t size = decompress(inputFile, inputSize, &decompressed);
			free(inputFile);
			if (!size) {
				fprintf(stderr, "%s doesn't seem to be a valid file.", inputFilename);
				return EXIT_FAILURE;
			}
			const bool failed = writeFile(outputFilename, decompressed, size);
			free(decompressed);
			if (failed) {
				printFileError(outputFilename, invocation);
				return EXIT_FAILURE;
			}
		}
		else {
			fprintf(stderr, "Unsupported mode: \"%s\"\n", mode);
			return printSyntax(invocation);
		}
	}
}