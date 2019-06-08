// this code reads from a UART line and parses BigRedBee GPS sentances

#ifndef BIG_RED_BEE
#define BIG_RED_BEE

#include "Arduino.h"

//#define SERIAL_DEBUGING

class BigRedBee {
  public:
    HardwareSerial *radio_serial;
    float longitude, latitude;
    unsigned long time; 
    int altitude, num_sats;
    char status;
    char hex_voltage[4];
    int Vbat; 
    float HDOP, VDOP;
    
    float minutes_to_decimal (float minutes); // converts gps hours and minutes to decimal degrees

    BigRedBee(HardwareSerial *serial_to_use);
    
    void begin(int baud);
    
    void parse_data();
};

#endif
