#include <Arduino.h>
#include <Adafruit_SharpMem.h>

#include "compass.h"
#include "gps.h"
#include "display.h"
#include "telemetry.h"

static constexpr int SHARP_SS = 5;
static const int BUTTONS[] = {A1, A2, A3};
static constexpr int LED = 13;

Adafruit_SharpMem display(SCK, MOSI, SHARP_SS, DISPLAY_W, DISPLAY_H);

#define BEGIN_DEBOUNCE(btn) \
do { \
   static long pressed = 0; \
   long now = millis(); \
   if(!analogRead(btn)) { \
      if(now - pressed > 20) { \
         pressed = now; \
      };
#define END_DEBOUNCE }} while(0)

static void a1Handler() {
   BEGIN_DEBOUNCE(A1);
   compass_calibrate_toggle();
   END_DEBOUNCE;
}

static void a2Handler() {
}

static void a3Handler() {
   BEGIN_DEBOUNCE(A3);
   TeleMode mode = tele_get_mode();
   tele_set_mode(mode == TELE_MODE_RADIO ? TELE_MODE_SERIAL : TELE_MODE_RADIO);
   END_DEBOUNCE;
}

static void (*buttonHandlers[])() = {a1Handler, a2Handler, a3Handler};

void setup() {
   // enable float in printf
   asm(".global _printf_float");

   // serial
   Serial.begin(9600);

   // buttons
   int i = 0;
   for(int button : BUTTONS) {
      pinMode(button, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(button), buttonHandlers[i++], CHANGE);
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
