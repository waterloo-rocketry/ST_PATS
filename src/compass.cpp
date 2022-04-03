#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <FlashStorage.h>

#include "display.h"
#include "compass.h"
#include "coordinate.h"
#include "gps.h"
#include "util.h"

static constexpr int IMU_SS = 10;
static Adafruit_LIS2MDL imu;

// used to approximate offset between true north and magnetic north
// https://en.wikipedia.org/wiki/Geomagnetic_pole
static Coordinate mag_north{
   .lat = 80.7 / 360 * TWO_PI,
   .lon = -72.7 / 360 * TWO_PI,
};
static Coordinate true_north{
   .lat = PI / 4, // 90 deg lat = north pole
   .lon = 0,
};

// for compass calibration
struct Calibration {
   float minx, miny, minz, maxx, maxy, maxz;
};

static bool calibrating = false;
static Calibration cal;
FlashStorage(calStorage, Calibration);

void compass_init() {
   compass_calibrate_load();
   imu.begin_SPI(IMU_SS, SCK, MISO, MOSI);
}

// return radians
float compass_heading() {
   sensors_event_t event;
   imu.getEvent(&event);

   float x = fmap(event.magnetic.x, cal.minx, cal.maxx, -1, 1);
   float y = fmap(event.magnetic.y, cal.miny, cal.maxy, -1, 1);

   return atan2(y, x);
}

void compass_calibrate_load() {
   calStorage.read(&cal);
}

void compass_calibrate_start() {
   cal.minx = cal.miny = cal.minz = INFINITY;
   cal.maxx = cal.maxy = cal.maxz = -INFINITY;
   calibrating = true;
}

void compass_calibrate_stop() {
   calibrating = false;
   calStorage.write(cal);
}

void compass_calibrate_toggle() {
   if(calibrating)
      compass_calibrate_stop();
   else
      compass_calibrate_start();
}

void compass_do_calibrate() {
   sensors_event_t event;
   imu.getEvent(&event);

   float x =  event.magnetic.x;
   float y =  event.magnetic.y;
   float z = -event.magnetic.y; // board is upside down

   if(x < cal.minx) cal.minx = x;
   if(x > cal.maxx) cal.maxx = x;
   if(y < cal.miny) cal.miny = y;
   if(y > cal.maxy) cal.maxy = y;
   if(z < cal.minz) cal.minz = z;
   if(z > cal.maxz) cal.maxz = z;

   display.setCursor(0, 0);
   display.println("Calibrating");

   display.print("min x ");
   display.println(cal.minx);
   display.print("max x ");
   display.println(cal.maxx);

   display.print("min y ");
   display.println(cal.miny);
   display.print("max y ");
   display.println(cal.maxy);

   display.print("min z ");
   display.println(cal.minz);
   display.print("max z ");
   display.println(cal.maxz);
}

void compass_update() {
   if(calibrating) {
      compass_do_calibrate();
      return;
   }

   const Coordinate &here = gps_coord();
   float offset = coord_angle_between(here, true_north, mag_north); // approximation
   float heading = compass_heading() + offset;

   static const int fontScale = 2;
   static const float radius = DISPLAY_H * 3 / 8;
   static float centerX = radius + 10, centerY = radius + 10;
   static const char cardinals[] = { 'N', 'E', 'S', 'W' };

   for(int cardinal = 0; cardinal < 4; cardinal++) {
      float x = centerX + (radius - fontScale * 5) * cos(cardinal * TWO_PI / 4 - heading) - fontScale * 5 / 2;
      float y = centerY + (radius - fontScale * 5) * sin(cardinal * TWO_PI / 4 - heading) - fontScale * 8 / 2;
      display.drawChar(x, y, cardinals[cardinal], 0, 1, fontScale);
   }

   for(int i = 0; i < fontScale; i++) {
      display.drawCircle(centerX, centerY, radius + i, 0);
   }

   display.setCursor(60, DISPLAY_H - 30);
   display.print(fmod((heading + TWO_PI) / TWO_PI * 360.0, 360));
   display.print("\xF8");
}
