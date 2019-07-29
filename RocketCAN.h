#ifndef ROCKET_CAN_H
#define ROCKET_CAN_H

#include <stdint.h>
#include <stdlib.h>
#include "Arduino.h"
#include "sd_handler.h"

#define GPS_MSG_LEN 11
#define GPS_MSG_HEADER '$'

class RocketCAN {
  public:

    HardwareSerial *radio_serial;
    double latitude;
    double longitude;
    long time_of_last_msg;
    uint8_t latitude_deg;
    uint8_t latitude_min;
    uint8_t latitude_dmin;
    uint8_t latitude_dir;
    uint8_t longitude_deg;
    uint8_t longitude_min;
    uint8_t longitude_dmin;
    uint8_t longitude_dir;

  RocketCAN(HardwareSerial *serial_to_use);
  /*
 * begins the UART line being used by the RocketCAN. 
 */
  void begin(int baud);

  /*
   * checks if there is a char to read then reads it and procceses it  if possible.
   */
  bool read();

  /*
   * returns how long its been since we got a new GPS message in ms
   */
  long time_since_last_msg();

  /*
   * logs all the RocketCAN GPS data to SD
   */
  void log_to_sd();

   
  private:

/*
 * This function takes an incoming char from the radio and puts it in it's place in the GPS msg string
 */
void process_incoming_char(char c);

/*
 * This function converts a binary value from 0 to 63 inclusive into a
 * printable charcter using a modified version of Base64. The + character is
 * not used, and is replaced by the & character, due to the XBEE interpreting
 * the + character as a special character.
 */
char binary_to_base64(uint8_t binary);

/*
 * This function converts a value from a modified version of Base64 into a raw
 * binary value. See the description of the function binary_to_base64() for a
 * description of the modified Base64 encoding and the reason for its use.
 */
uint8_t base64_to_binary(char base64);

/*
 * Unpacks str into latitude and longitude values. str must be a buffer
 * at least GPS_MSG_LEN bytes long. Returns true on success.
 */
bool expand_gps_message(uint8_t *latitude_deg,
                        uint8_t *latitude_min,
                        uint8_t *latitude_dmin,
                        uint8_t *latitude_dir,
                        uint8_t *longitude_deg,
                        uint8_t *longitude_min,
                        uint8_t *longitude_dmin,
                        uint8_t *longitude_dir,
                        char *str);

/*
 * This function computes the checksum of a NULL-terminated message using a
 * modified version of the Luhn algorithm. The checksum is equal to the sum of
 * the odd-placed digits plus three times the sum of the even-placed digits,
 * modulo 64. The function returns the Base-64 encoding of the checksum.
 */
char checksum(char *cmd);

/*
 * converts the elegant series of variables used to convey location into 2 doubles.
 */
void minutes_to_degrees();
};

#endif
