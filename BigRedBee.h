// this code reads from Serial1 and parses BigRedBee GPS sentances

#ifndef BIG_RED_BEE
#define BIG_RED_BEE

#include "Arduino.h"

//#define SERIAL_DEBUGING

class BigRedBee {
  private:
    HardwareSerial *radio_serial;
    float longditude, latitude;
    unsigned long time; 
    int altitude, num_sats;
    char status;
    char hex_voltage[4];
    int Vbat; 
    float HDOP, VDOP;
    
    float minutes_to_decimal (float minutes); // converts gps hours and minutes to decimal degrees

  public:
    BigRedBee(HardwareSerial *serial_to_use);
    
    void begin(int baud);
    
    void parse_data();
};

#endif
