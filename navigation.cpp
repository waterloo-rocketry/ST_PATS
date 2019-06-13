#include "navigation.h"

double target_heading(double your_lat, double your_long, double target_lat, double target_long){
  double heading = (atan2(-(target_lat - your_lat), target_long - your_long)*180 / PI) + 90;
  if (heading < 0){
   heading = 360 + heading;
  }
  return heading;
}

double distance_to_target_lat(double your_lat, double target_lat){
  return abs(target_lat - your_lat)*111000; // in meters
}

double distance_to_target(double your_lat, double your_long, double target_lat, double target_long){
  double lat_distance = abs(target_lat - your_lat)*111000; // in meters
  double long_distance = cos((your_lat + target_lat)/2)*111321; // in meters
  return sqrt(pow(lat_distance, 2) + pow(long_distance, 2));
}
