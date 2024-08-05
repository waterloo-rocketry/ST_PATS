#pragma once
#ifdef __cplusplus
extern "C" {
#endif
uint8_t crc8_checksum(uint8_t *pdata, size_t nbytes, uint8_t crc);
#ifdef __cplusplus
}
#endif
