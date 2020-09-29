#include "EEPROM_storage.h"

void save_GPS_location(double latitude, double longitude, int index) {
  EEPROM.put(index*16, latitude);
  EEPROM.put(index*16 + 8, longitude);
  
}

void get_GPS_location(double &latitude, double &longitude, int index) {
  EEPROM.get(index*16, latitude);
  EEPROM.get(index*16 + 8, longitude);
}
