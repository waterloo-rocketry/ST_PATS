#include <Arduino.h>
#include <Adafruit_SharpMem.h>

#include "compass.h"
#include "gps.h"
#include "display.h"

static constexpr int SHARP_SS = 5;

Adafruit_SharpMem display(SCK, MOSI, SHARP_SS, DISPLAY_W, DISPLAY_H);

void buttonHandler() {
   // TODO
}

void setup() {
   // enable float in printf
   asm(".global _printf_float");

   // buttons
   static const int buttons[] = { A1, A2, A3 };
   for(int button : buttons) {
      pinMode(button, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(button), buttonHandler, CHANGE);
   }

   // led output
   pinMode(13, OUTPUT);

   // display
   display.begin();
   display.cp437(true);
   display.setTextSize(2);
   display.setTextColor(0);
   display.setRotation(2);

   // other
   compass_init();
   gps_init();
}

void loop() {
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
