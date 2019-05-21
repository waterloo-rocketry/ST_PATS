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
  heading = (atan2(event.magnetic.y,event.magnetic.x+17.5) * 180) / PI;
  
  // Normalize to 0-360
  if (heading < 0)
   {
   heading = 360 + heading;
   }
}

float Compass::get_heading(){
  return heading;
}
