#include "RocketCAN.h"

RocketCAN::RocketCAN(HardwareSerial *serial_to_use){
    radio_serial = serial_to_use;
    latitude = 0;
    longitude = 0;
    time_of_last_msg = 0;
    latitude_deg = 0;
    latitude_min = 0;
    latitude_dmin = 0;
    latitude_dir = 0;
    longitude_deg = 0;
    longitude_min = 0;
    longitude_dmin = 0;
    longitude_dir = 0;
}

void RocketCAN::begin(int baud){
  radio_serial->begin(baud);
}

bool RocketCAN::read(){
  if(radio_serial->available() > 0){
    process_incoming_char(radio_serial->read());
    return true;
  }
  return false;
}

void RocketCAN::process_incoming_char(char c) {
    static enum {
        NONE,
        GPS_READ
    } state;
    static char gps_coords_buffer[GPS_MSG_LEN] = {0};
    static uint8_t buffer_index = 0;

    if (c == GPS_MSG_HEADER) {
        buffer_index = 0;
        state = GPS_READ;
        gps_coords_buffer[buffer_index++] = c;
    }

    else if (state == GPS_READ) {
        gps_coords_buffer[buffer_index++] = c;
        if(buffer_index == GPS_MSG_LEN + 1) {
            state = NONE;
            expand_gps_message(&latitude_deg,
                               &latitude_min,
                               &latitude_dmin,
                               &latitude_dir,
                               &longitude_deg,
                               &longitude_min,
                               &longitude_dmin,
                               &longitude_dir,
                               gps_coords_buffer);
        }
    }
}

bool RocketCAN::expand_gps_message(uint8_t *latitude_deg,
                        uint8_t *latitude_min,
                        uint8_t *latitude_dmin,
                        uint8_t *latitude_dir,
                        uint8_t *longitude_deg,
                        uint8_t *longitude_min,
                        uint8_t *longitude_dmin,
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

    *longitude_deg   = (base64_to_binary(str[5]) << 2) & 0xfc;
    *longitude_deg  |= (base64_to_binary(str[6]) >> 4) & 0x3;

    *longitude_min   = (base64_to_binary(str[6]) << 4) & 0xf0;
    *longitude_min  |= (base64_to_binary(str[7]) >> 2) & 0xf;

    *longitude_dmin  = (base64_to_binary(str[7]) << 6) & 0xc0;
    *longitude_dmin |= (base64_to_binary(str[8]))      & 0x30;

    // last char has lat_dir (1=='N', 0=='S') and lon_dir (1=='E', 0==W)
    *latitude_dir  = (base64_to_binary(str[9]) & 0x20) ? 'N' : 'S';
    *longitude_dir = (base64_to_binary(str[9]) & 0x10) ? 'E' : 'W';

    minutes_to_degrees();

    time_of_last_msg = millis();

    Serial.println(*latitude_deg);
    Serial.println(*latitude_min);
    Serial.println(*latitude_dmin);
    Serial.println((char)*latitude_dir);
    Serial.println(*longitude_deg);
    Serial.println(*longitude_min);
    Serial.println(*longitude_dmin);
    Serial.println((char)*longitude_dir);
    Serial.println(latitude, 6);
    Serial.println(longitude, 6);


    return true;
}

char RocketCAN::binary_to_base64(uint8_t binary)
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

uint8_t RocketCAN::base64_to_binary(char base64)
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

char RocketCAN::checksum(char *cmd)
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

void RocketCAN::minutes_to_degrees(){
  double latitude_minutes = latitude_min + (latitude_dmin / 100.0);
  latitude = latitude_deg + latitude_minutes / 60;
  if(latitude_dir == 'S'){
    latitude *= -1;
  }
  double longitude_minutes = longitude_min + (longitude_dmin / 100.0);
  longitude = longitude_deg + longitude_minutes / 60;
  if(longitude_dir == 'W'){
    longitude *= -1;
  }
}

long RocketCAN::time_since_last_msg(){
  return millis() - time_of_last_msg;
}
