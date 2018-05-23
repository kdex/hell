#pragma once
#include "common/no-return.h"
#include "common/restrict.h"
#include "common/types.h"
#include <stdbool.h>
#include <stdlib.h>
size_t readFile(const char *filename, u8 **buffer);
bool writeFile(const char *filename, const u8 *buffer, size_t bufferLength);
NO_RETURN void fail(const char *RESTRICT format, ...);
NO_RETURN void failWithFileError(const char *filename, const char *invocation);