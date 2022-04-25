// This file is from https://github.com/waterloo-rocketry/cansw_radio/blob/32596bdc1e8dafad1fca0e2b322a53e3afc48afd/serialize.c
#include "can_radio.h"
#include <string.h>
#include <stddef.h>

char binary_to_base64(uint8_t binary)
{
    if (binary <= 25)
        return binary + 'A';
    if (binary <= 51)
        return (binary - 26) + 'a';
    if (binary <= 61)
        return (binary - 52) + '0';
    if (binary == 62)
        return '&';
    if (binary == 63)
        return '/';
    // ***TODO***: return an error in all other cases
    return 0;
}

uint8_t base64_to_binary(char base64)
{
    if ('A' <= base64 && base64 <= 'Z')
        return base64 - 'A';
    if ('a' <= base64 && base64 <= 'z')
        return base64 - 'a' + 26;
    if ('0' <= base64 && base64 <= '9')
        return base64 - '0' + 52;
    if (base64 == '&')
        return 62;
    if (base64 == '/')
        return 63;
    // ***TODO***: return an error in all other cases
    return 255;
}

bool expand_gps_message(uint8_t *latitude_deg,
                        uint8_t *latitude_min,
                        uint16_t *latitude_dmin,
                        uint8_t *latitude_dir,
                        uint8_t *longitude_deg,
                        uint8_t *longitude_min,
                        uint16_t *longitude_dmin,
                        uint8_t *longitude_dir,
                        char *str)
{
    if (str == NULL || str[0] != GPS_MSG_HEADER) {
        return false;
    }

    char actual_checksum = str[GPS_MSG_LEN - 1];
    str[GPS_MSG_LEN - 1] = '\0';
    char expected_checksum = checksum(str);
    if (expected_checksum != actual_checksum) {
        return false;
    }

    // 6 bits per char. So with 6 bytes, it's gonna take 8 characters
    *latitude_deg   = (base64_to_binary(str[1]) << 2) & 0xfc;
    *latitude_deg  |= (base64_to_binary(str[2]) >> 4) & 0x3;

    *latitude_min   = (base64_to_binary(str[2]) << 4) & 0xf0;
    *latitude_min  |= (base64_to_binary(str[3]) >> 2) & 0xf;

    *latitude_dmin  = (base64_to_binary(str[3]) << 6) & 0xc0;
    *latitude_dmin |= (base64_to_binary(str[4]))      & 0x3f;

    *latitude_dmin <<= 8;

    *latitude_dmin  |= (base64_to_binary(str[5]) << 2) & 0xfc;
    *latitude_dmin  |= (base64_to_binary(str[6]) >> 4) & 0x3;

    *longitude_deg   = (base64_to_binary(str[6]) << 4) & 0xf0;
    *longitude_deg  |= (base64_to_binary(str[7]) >> 2) & 0xf;

    *longitude_min   = (base64_to_binary(str[7]) << 6) & 0xc0;
    *longitude_min  |= (base64_to_binary(str[8]))      & 0x3f;

    *longitude_dmin |= (base64_to_binary(str[9]) << 2) & 0xfc;
    *longitude_dmin |= (base64_to_binary(str[10]) >> 4) & 0x3;

    *longitude_dmin <<= 8;

    *longitude_dmin |= (base64_to_binary(str[10]) << 4) & 0xf0;
    *longitude_dmin |= (base64_to_binary(str[11]) >> 2) & 0xf;

    // last char has lat_dir (1=='N', 0=='S') and lon_dir (1=='E', 0==W)
    *latitude_dir  = (base64_to_binary(str[11]) & 0x2) ? 'N' : 'S';
    *longitude_dir = (base64_to_binary(str[11]) & 0x1) ? 'E' : 'W';

    return true;
}

char checksum(char *cmd)
{
    uint8_t total = 0;
    uint8_t idx = 0;
    while (cmd[idx] != 0) {
        uint8_t curr = (uint8_t) cmd[idx];
        uint8_t odd_sum = 0;
        uint8_t even_sum = 0;
        for (uint8_t i = 0; i < 4; ++i) {
            odd_sum += 0b00000010 & curr;
            even_sum += 0b00000001 & curr;
            curr = curr >> 2;
        }
        total += odd_sum + 3 * even_sum;
        ++idx;
    }
    total %= 64;
    return binary_to_base64(total);
}
