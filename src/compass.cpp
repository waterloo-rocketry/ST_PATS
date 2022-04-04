#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <FlashStorage.h>

#include "display.h"
#include "compass.h"
#include "gps.h"
#include "telemetry.h"
#include "util.h"

static constexpr int IMU_SS = 10;
static Adafruit_LIS2MDL imu;

// for compass calibration
struct Calibration {
   float minx, miny, minz, maxx, maxy, maxz;
};

static bool calibrating = false;
static Calibration cal;
FlashStorage(calStorage, Calibration); // macro

void compass_init() {
   compass_calibrate_load();
   imu.begin_SPI(IMU_SS, SCK, MISO, MOSI);
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

   // calculate heading from sensors and gps
   sensors_event_t event;
   imu.getEvent(&event);

   float x = fmap(event.magnetic.x, cal.minx, cal.maxx, -1, 1);
   float y = fmap(event.magnetic.y, cal.miny, cal.maxy, -1, 1);
   float heading = atan2(y, x) + gps_magvariation();

   // calculate target direction
   float olat, olon, tlat, tlon; // origin, target
   gps_coord(olat, olon);
   tele_coord(tlat, tlon);
   float targetHeading = heading_from(olat, olon, tlat, tlon);

   // display
   static constexpr int fontScale = 2;
   static constexpr float radius = 72;
   static constexpr float centerX = radius + 10, centerY = radius + 10;

   // letters to show around the compass
   static const char cardinals[] = {'N', 'W', 'S', 'E'};

   // triangles that make up the arrow, in polar coordinate th, r
   static const float arrow[][3][2] = {
      {
         {TWO_PI / 4, radius * 0.7},
         {TWO_PI * 11 / 16, radius * 0.6},
         {TWO_PI * 3 / 4, radius * 0.2},
      },
      {
         {TWO_PI / 4, radius * 0.7},
         {TWO_PI * 13 / 16, radius * 0.6},
         {TWO_PI * 3 / 4, radius * 0.2},
      }
   };

   // draw cardinal directions
   for(int cardinal = 0; cardinal < 4; cardinal++) {
      float x = centerX + (radius - fontScale * 5) * cos(cardinal * TWO_PI / 4 + heading) - fontScale * 5 / 2;
      float y = centerY - (radius - fontScale * 5) * sin(cardinal * TWO_PI / 4 + heading) - fontScale * 8 / 2;
      display.drawChar(x, y, cardinals[cardinal], 0, 1, fontScale);
   }

   // draw arrow
   if(gps_fixed()) {
      for(size_t i = 0; i < sizeof(arrow) / sizeof(arrow[0]); i++) {
         display.fillTriangle(
            centerX + arrow[i][0][1] * cos(arrow[i][0][0] + heading - targetHeading),
            centerY - arrow[i][0][1] * sin(arrow[i][0][0] + heading - targetHeading),
            centerX + arrow[i][1][1] * cos(arrow[i][1][0] + heading - targetHeading),
            centerY - arrow[i][1][1] * sin(arrow[i][1][0] + heading - targetHeading),
            centerX + arrow[i][2][1] * cos(arrow[i][2][0] + heading - targetHeading),
            centerY - arrow[i][2][1] * sin(arrow[i][2][0] + heading - targetHeading),
            0
         );
      }
   }

   // draw outer circle
   for(int i = 0; i < fontScale; i++) {
      display.drawCircle(centerX, centerY, radius + i, 0);
   }

   /*
   // debug circle
   display.fillCircle(
      centerX + fmap(event.magnetic.x, cal.minx, cal.maxx, -radius, radius),
      centerY - fmap(event.magnetic.y, cal.miny, cal.maxy, -radius, radius),
      5, 0
   );
   */

   // draw current heading
   {
      int x = 45, y = DISPLAY_H - 60;
      char buff[8];
      display.setCursor(x, y);
      snprintf(buff, sizeof(buff), "%6.2f\xF8", fmod((heading + TWO_PI) / TWO_PI * 360.0, 360));
      display.print(buff);

      // draw target heading
      display.setCursor(x, y + LINE_H);
      if(gps_fixed()) {
         snprintf(buff, sizeof(buff), "%6.2f\xF8", fmod((targetHeading + TWO_PI) / TWO_PI * 360.0, 360));
         display.print(buff);
      }
   }
}
