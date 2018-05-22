#pragma once
#include "core/compression-mode.h"
#include "common/types.h"
#include <stdlib.h>
typedef struct HeaderInternals HeaderInternals;
typedef struct {
	u8 size;
	u16 capacity;
	HeaderInternals *restrict internals;
} Header;
u8 makeFirstByte(const Header *restrict header, CompressionMode mode, u16 size);
void initSmallHeader(Header *restrict header);
void initLargeHeader(Header *restrict header);
void freeHeader(Header *restrict header);