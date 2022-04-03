#pragma once
struct Coordinate;

void gps_init();
void gps_update();
const Coordinate &gps_coord();
