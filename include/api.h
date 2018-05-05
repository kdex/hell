#pragma once
struct HALInterface {
	size_t (*decompress)(const uint8_t *compressed, uint8_t **decompressed);
};
extern const struct HALInterface HAL;