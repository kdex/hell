#include "common/constants.h"
#include "common/types.h"
#include "core/compress.h"
#include "core/decompress.h"
#include "core/make-meta/meta.h"
#include "io/io.h"
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
	const char *mode = argv[1];
	if (!strcmp(mode, "-v") || !strcmp(mode, "--version")) {
		printf("%s version %s\n", projectName, projectVersion);
		return EXIT_SUCCESS;
	}
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
			failWithFileError(inputFilename, invocation);
		}
		if (inputSize > MAX_PAYLOAD) {
			fail("Can not compress %s (%lu bytes), which is larger than maximum allowed file size (%u bytes)\n", inputFilename, inputSize, MAX_PAYLOAD);
		}
		u8 *compressed;
		const size_t size = compress(inputFile, inputSize, &compressed);
		free(inputFile);
		if (!size) {
			fail("%s doesn't seem to be a valid file.\n", inputFilename);
		}
		const float savings = 100 * (1 - (float) size / inputSize);
		printf("Reduced %s by %li bytes (%.2f %%)\n", inputFilename, inputSize - size, savings);
		const bool failed = writeFile(outputFilename, compressed, size);
		free(compressed);
		if (failed) {
			failWithFileError(outputFilename, invocation);
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
			failWithFileError(inputFilename, invocation);
		}
		u8 *decompressed;
		const size_t size = decompress(inputFile, inputSize, &decompressed);
		free(inputFile);
		if (!size) {
			fail("%s doesn't seem to be a valid file.\n", inputFilename);
		}
		const bool failed = writeFile(outputFilename, decompressed, size);
		free(decompressed);
		if (failed) {
			failWithFileError(outputFilename, invocation);
		}
	}
	else {
		fprintf(stderr, "Unsupported mode: \"%s\"\n", mode);
		return printSyntax(invocation);
	}
}