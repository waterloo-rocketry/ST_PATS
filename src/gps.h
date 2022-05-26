#pragma once
struct Time {
   uint8_t hour, minute, second;
};

void gps_init();
void gps_update();
void gps_coord(float &lat, float &lon); //radians
bool gps_fixed();
void gps_time(Time &time);
float gps_magvariation(); // radians
