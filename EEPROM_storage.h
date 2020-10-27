#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <EEPROM.h>

struct EEPROM_data{
  bool nav = false;
double latitude = 0;
double longitude = 0;

};

void save_GPS_location(double latitude, double longitude, int index);

void get_GPS_location(double &latitude, double &longitude, int index);

#endif
