#pragma once
#include "types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdnoreturn.h>
size_t readFile(const char *filename, u8 **buffer);
bool writeFile(const char *filename, const u8 *buffer, size_t bufferLength);
noreturn void fail(const char *restrict format, ...);
noreturn void failWithFileError(const char *filename, const char *invocation);