#pragma once
#include "core/compression-mode.h"
#include "common/restrict.h"
#include "common/types.h"
#include <stdlib.h>
typedef struct HeaderInternals HeaderInternals;
typedef struct {
	u8 size;
	u16 capacity;
	HeaderInternals *RESTRICT internals;
} Header;
u8 makeFirstByte(const Header *RESTRICT header, CompressionMode mode, u16 size);
void initSmallHeader(Header *RESTRICT header);
void initLargeHeader(Header *RESTRICT header);
void freeHeader(Header *RESTRICT header);