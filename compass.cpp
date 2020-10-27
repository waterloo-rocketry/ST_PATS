#include "compass.h"

Compass::Compass(){
  heading = 0;
}
bool Compass::begin(){
  compass.begin();
}

void Compass::read(){
  sensors_event_t event; 
  compass.getEvent(&event);
  double corrected_x = map(event.magnetic.x, X_MIN, X_MAX, -100, 100);
  double corrected_y = map(event.magnetic.y, Y_MIN, Y_MAX, -100, 100);
  double corrected_z = map(event.magnetic.z, Z_MIN, Z_MAX, -100, 100);
  heading = (atan2(corrected_y, corrected_x) * 180) / PI;
  
  // Normalize to 0-360
  if (heading < 0)
   {
   heading = 360 + heading;
   }
}

double Compass::get_heading(){
  return heading;
}
