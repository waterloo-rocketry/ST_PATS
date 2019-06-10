// this code reads from a UART line and parses BigRedBee GPS sentances

#ifndef BIG_RED_BEE_H
#define BIG_RED_BEE_H

#include "Arduino.h"

//#define SERIAL_DEBUGING

class BigRedBee {
  public:
    HardwareSerial *radio_serial;
    double longitude, latitude;
    unsigned long time; 
    int altitude, num_sats;
    char status;
    char hex_voltage[4];
    int Vbat; 
    double HDOP, VDOP;
    
    double minutes_to_decimal (double minutes); // converts gps hours and minutes to decimal degrees

    BigRedBee(HardwareSerial *serial_to_use);
    
    void begin(int baud);
    
    void parse_data();
};

#endif
