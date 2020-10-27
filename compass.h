#ifndef COMPASS_H
#define COMPASS_H

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

#define X_MIN -60.55
#define X_MAX 66.55
#define Y_MIN -104.09
#define Y_MAX 71.00
#define Z_MIN -134.90
#define Z_MAX  33.57

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
