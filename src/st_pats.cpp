#include <Arduino.h>
#include <Adafruit_SharpMem.h>

#include "compass.h"
#include "gps.h"
#include "display.h"
#include "telemetry.h"

static constexpr int SHARP_SS = 5;
static const int BUTTONS[] = {A1, A2, A3};

Adafruit_SharpMem display(SCK, MOSI, SHARP_SS, DISPLAY_W, DISPLAY_H);

#define BEGIN_DEBOUNCE \
do { \
   static long pressed = 0; \
   long now = millis(); \
   if(now - pressed > 200) {
#define END_DEBOUNCE \
   } \
   pressed = now; \
} while(0)

// button 1 toggles compass calibration
static void a1Handler() {
   BEGIN_DEBOUNCE;
   compass_calibrate_toggle();
   END_DEBOUNCE;
}

// button 2 saves current telemetry coordinate to flash
static void a2Handler() {
   BEGIN_DEBOUNCE;
   tele_save();
   END_DEBOUNCE;
}

// button 3 toggles telemetry mode
static void a3Handler() {
   BEGIN_DEBOUNCE;
   tele_set_mode(tele_get_mode() == TELE_MODE_RADIO ? TELE_MODE_SERIAL : TELE_MODE_RADIO);
   END_DEBOUNCE;
}

static void (*buttonHandlers[])() = {a1Handler, a2Handler, a3Handler};

void setup() {
   // enable float in printf
   asm(".global _printf_float");

   // buttons
   int i = 0;
   for(int button : BUTTONS) {
      pinMode(button, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(button), buttonHandlers[i++], FALLING);
   }

   // led output
   pinMode(LED, OUTPUT);

   // display
   display.begin();
   display.cp437(true);
   display.setTextSize(2);
   display.setTextColor(0);
   display.setRotation(2);

   // other
   gps_init();
   tele_init();
   compass_init();
}

void loop() {
   display.clearDisplayBuffer();

   gps_update();
   compass_update();
   tele_update();

   display.refresh();
}
