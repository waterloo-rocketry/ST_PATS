#pragma once

static constexpr float RADIUS_OF_EARTH = 6371088; // m

// linearly maps a float between [from1, from2] and [to1, to1]
inline float fmap(float v, float from1, float from2, float to1, float to2) {
   return to1 + (v - from1) / (from2 - from1) * (to2 - to1);
}

// project an coordinate with azimuthal equidistant projection centered on an origin.
// north is 0 and clock wise is positive, in radians.
// angles in rads, distance in m
inline void project_azimuth(float olat, float olon, float tlat, float tlon, float &th, float &rho) {
   // equation from https://en.wikipedia.org/wiki/Azimuthal_equidistant_projection#Mathematical_definition
   float colat = cos(olat), solat = sin(olat);
   float ctlat = cos(tlat), stlat = sin(tlat);
   float cto = cos(tlon - olon);
   th = atan2(ctlat * sin(tlon - olon), (colat * stlat - solat * ctlat * cto));

   // doesn't work with 4 byte float when distance is small
   //rho = acos(solat * stlat + colat * ctlat * cto) * RADIUS_OF_EARTH;

   // use trig instead
   double latdist = abs(tlat - olat) / TWO_PI * 360 * 111000;
   double londist = abs(tlon - olon) * cos((olat + tlat) / 2) / TWO_PI * 360 * 111321;
   rho = sqrt(latdist * latdist + londist * londist);
}
