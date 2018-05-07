#pragma once
#include "types.h"
#include <stdbool.h>
#include <stdlib.h>
void printFileError(const char *filename, const char *invocation);
size_t readFile(const char *filename, u8 **buffer);
bool writeFile(const char *filename, const u8 *buffer, u8 bufferLength);