#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>

#include "config.h"
#include "display.h"
#include "compass.h"
#include "util.h"

static Adafruit_LIS2MDL imu;

static bool calibrating = false;
static float minx, miny, minz, maxx, maxy, maxz;

void compass_init() {
   imu.begin_SPI(IMU_SS, SCK, MISO, MOSI);
   compass_calibrate_start();
}

// return radians
double compass_heading() {
   sensors_event_t event;
   imu.getEvent(&event);

   float x = fmap(event.magnetic.x, minx, maxx, -1, 1);
   float y = fmap(event.magnetic.y, miny, maxy, -1, 1);

   return atan2(y, x);
}

void compass_calibrate_start() {
   minx = miny = minz = INFINITY;
   maxx = maxy = maxz = -INFINITY;
   calibrating = true;
}

void compass_calibrate_stop() {
   calibrating = false;
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

   if(x < minx) minx = x;
   if(x > maxx) maxx = x;
   if(y < miny) miny = y;
   if(y > maxy) maxy = y;
   if(z < minz) minz = z;
   if(z > maxz) maxz = z;

   display.println("Calibrating");

   display.print("x: ");
   display.print(minx);
   display.print(", ");
   display.println(maxx);

   display.print("y: ");
   display.print(miny);
   display.print(", ");
   display.println(maxy);

   display.print("z: ");
   display.print(minz);
   display.print(", ");
   display.println(maxz);
}

void compass_update() {
   display.setCursor(0, 0);

   if(calibrating) {
      compass_do_calibrate();
      return;
   }

   display.print("heading: ");
   display.print(compass_heading() / TWO_PI * 360.0);
}
