#pragma once
#include <stdint.h>
#include <stdbool.h>

#define GPS_MSG_LEN 11
#define GPS_MSG_HEADER '$'

#ifdef __cplusplus
extern "C" {
#endif

char binary_to_base64(uint8_t binary);
uint8_t base64_to_binary(char base64);
bool create_gps_message(uint8_t latitude_deg, uint8_t latitude_min, uint8_t latitude_dmin, uint8_t latitude_dir, uint8_t longitude_deg, uint8_t longitude_min, uint8_t longitude_dmin, uint8_t longitude_dir, char *str);
bool expand_gps_message(uint8_t *latitude_deg, uint8_t *latitude_min, uint8_t *latitude_dmin, uint8_t *latitude_dir, uint8_t *longitude_deg, uint8_t *longitude_min, uint8_t *longitude_dmin, uint8_t *longitude_dir, char *str);
char checksum(char *cmd);

#ifdef __cplusplus
}
#endif
