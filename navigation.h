#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Arduino.h"

double target_heading(double your_lat, double your_long, double target_lat, double target_long);
double distance_to_target(double your_lat, double your_long, double target_lat, double target_long);

#endif // !NAVIGATION_H
