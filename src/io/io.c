#include "io.h"
#include "common/types.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
size_t readFile(const char *filename, u8 **buffer) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		*buffer = NULL;
		return 0;
	}
	fseek(file, 0, SEEK_END);
	const long size = ftell(file);
	*buffer = malloc(size);
	rewind(file);
	fread(*buffer, 1, size, file);
	fclose(file);
	return size;
}
bool writeFile(const char *filename, const u8 *buffer, size_t bufferLength) {
	FILE *file = fopen(filename, "wb");
	if (!file) {
		return true;
	}
	else {
		fwrite(buffer, sizeof(u8), bufferLength, file);
		return false;
	}
}
noreturn void fail(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
noreturn void failWithFileError(const char *filename, const char *invocation) {
	const char *format = "%s: Cannot access \"%s\"";
	const size_t messageLength = snprintf(NULL, 0, format, invocation, filename);
	char *message = malloc(messageLength);
	snprintf(message, messageLength, format, invocation, filename);
	perror(message);
	free(message);
	exit(EXIT_FAILURE);
}