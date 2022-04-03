#pragma once
inline float fmap(float v, float from1, float from2, float to1, float to2) {
   return to1 + (v - from1) / (from2 - from1) * (to2 - to1);
}

inline float heading_from(float olat, float olon, float tlat, float tlon) {
   // equation from https://en.wikipedia.org/wiki/Azimuthal_equidistant_projection#Mathematical_definition
   float colat = cos(olat), solat = sin(olat);
   float ctlat = cos(tlat), stlat = sin(tlat);
   return atan2(ctlat * sin(tlon - olon), (colat * stlat - solat * ctlat * cos(tlon - olon)));
}
