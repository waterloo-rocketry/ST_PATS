#pragma once
struct Coordinate;

void gps_init();
void gps_update();
void gps_coord(float &lat, float &lon); //radians
bool gps_fixed();
float gps_magvariation(); // radians
