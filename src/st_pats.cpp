#include <Arduino.h>
#include <Adafruit_SharpMem.h>

#include "config.h"
#include "compass.h"
#include "gps.h"

Adafruit_SharpMem display(SCK, MOSI, SHARP_SS, 400, 240);

void setup(void) {
   pinMode(A1, INPUT_PULLUP);
   pinMode(A2, INPUT_PULLUP);
   pinMode(A3, INPUT_PULLUP);
   pinMode(13, OUTPUT);

   display.begin();
   display.setTextSize(2);
   display.setTextColor(0);
   display.setRotation(2);

   compass_init();
   gps_init();
}

void loop(void) {
   static bool a1_pressed = false;
   if(!digitalRead(A1)) {
      if(!a1_pressed) {
         compass_calibrate_toggle();
      }
      a1_pressed = true;
   } else {
      a1_pressed = false;
   }

   display.clearDisplayBuffer();

   compass_update();
   gps_update();

   display.refresh();
}
