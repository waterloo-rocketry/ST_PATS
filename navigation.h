#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Arduino.h"

// -9.65 in waterloo
// 8.31 at spaceport america
const float magnetic_declination =  -9.65;

double target_heading(double your_lat, double your_long, double target_lat, double target_long);
double distance_to_target(double your_lat, double your_long, double target_lat, double target_long);

#endif // !NAVIGATION_H
