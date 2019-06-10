#ifndef COMPASS_H
#define COMPASS_H

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

class Compass {
private:
Adafruit_LSM303_Mag_Unified compass = Adafruit_LSM303_Mag_Unified(12345); // assign ID of 12345
double heading;
public:
Compass();
bool begin();
void read();
double get_heading();

};


#endif
