#include <Arduino.h>
#include "coordinate.h"

void coord_from_deg(Coordinate &coord, float lat, float lon) {
   coord.lat = (float) (lat / 360 * TWO_PI);
   coord.lon = (float) (lon / 360 * TWO_PI);
}

float coord_angle_between(const Coordinate &origin, const Coordinate &a, const Coordinate &b) {
   // equation from https://en.wikipedia.org/wiki/Azimuthal_equidistant_projection#Mathematical_definition
   float colat = cos(origin.lat), solat = sin(origin.lat);
   float calat = cos(a.lat), salat = sin(a.lat);
   float cblat = cos(b.lat), sblat = sin(b.lat);
   float tha = atan2(calat * sin(a.lon - origin.lon), (colat * salat - solat * calat * cos(a.lon - origin.lon)));
   float thb = atan2(cblat * sin(b.lon - origin.lon), (colat * sblat - solat * cblat * cos(b.lon - origin.lon)));
   return thb - tha;
}
