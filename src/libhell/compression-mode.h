#pragma once
typedef enum {
	UNCOMPRESSED,
	FILL_BYTE,
	FILL_BYTES,
	FILL_INCREMENTAL_SEQUENCE,
	COPY_BYTES,
	COPY_REVERSED_BITS,
	COPY_REVERSED_BYTES,
	EXTEND_HEADER
} CompressionMode;