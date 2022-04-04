#include <Arduino.h>
#include <wiring_private.h> // pinPeripheral

#include "telemetry.h"
#include "display.h"

static constexpr int TELE_TX = 11;
static constexpr int TELE_RX = 12;

static Uart TeleSerial(&sercom1, TELE_RX, TELE_TX, SERCOM_RX_PAD_3, UART_TX_PAD_0);

static TeleMode mode = TELE_MODE_RADIO;
static float latDeg = 0;
static float lonDeg = 0;
static float alt = 0;

static void parsedeg(float degFloat, int &deg, int &min, float &sec) {
   deg = (int) degFloat;
   degFloat = (degFloat - deg) * 60;
   min = (int) degFloat;
   sec = (degFloat - min) * 60;
}

void SERCOM1_Handler() {
   TeleSerial.IrqHandler();
}

void tele_init() {
   // Telemetry breakout serial
   pinPeripheral(TELE_RX, PIO_SERCOM);
   pinPeripheral(TELE_TX, PIO_SERCOM);
   TeleSerial.begin(57600);

   // USB Serial should be initialized in setup() already
}

void tele_update() {
   // get gps from telemetry
   switch(mode) {
      case TELE_MODE_RADIO:
         // TODO
         break;
      case TELE_MODE_SERIAL:
         while(Serial.available()) {
            float num = Serial.parseFloat(SKIP_WHITESPACE);
            switch(Serial.read()) {
               case 'N':
                  latDeg = num;
                  break;
               case 'S':
                  latDeg = -num;
                  break;
               case 'E':
                  lonDeg = num;
                  break;
               case 'W':
                  lonDeg = -num;
                  break;
               case 'M':
                  alt = num;
                  break;
               default:
                  break;
            }
         }
         break;
   }

   // display
   int x = DISPLAY_W - 200;
   int y = DISPLAY_H - 100;

   display.setCursor(x, y);

   // display time
   display.print("Telemetry GPS ");
   switch(mode) {
      case TELE_MODE_RADIO:
         display.print("(Radio)");
         break;
      case TELE_MODE_SERIAL:
         display.print("(Serial)");
         break;
   }

   y += LINE_H;
   display.setCursor(x, y);

   char buff[24];
   int deg, min;
   float sec;

   parsedeg(fabs(latDeg), deg, min, sec);
   snprintf(buff, sizeof(buff), "LAT% 3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, latDeg >= 0 ? 'N' : 'S');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   parsedeg(fabs(lonDeg), deg, min, sec);
   snprintf(buff, sizeof(buff), "LON% 3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, lonDeg >= 0 ? 'E' : 'W');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   snprintf(buff, sizeof(buff), "ALT % 4.2fM", alt);
   display.print(buff);
}

void tele_coord(float &lat, float &lon) {
   lat = latDeg / 360 * TWO_PI;
   lon = lonDeg / 360 * TWO_PI;
}

void tele_set_mode(TeleMode _mode) {
   mode = _mode;
}

TeleMode tele_get_mode() {
   return mode;
}
