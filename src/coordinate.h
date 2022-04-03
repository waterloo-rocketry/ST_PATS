#pragma once

struct Coordinate {
   float lat, lon;
};

void coord_from_deg(Coordinate &coord, float lat, float lon);
float coord_angle_between(const Coordinate &origin, const Coordinate &a, const Coordinate &b);
