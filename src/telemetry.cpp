#include <Arduino.h>
#include <wiring_private.h> // pinPeripheral
#include <FlashStorage.h>

#include "telemetry.h"
#include "display.h"
#include "can_radio.h"
#include "gps.h"

static constexpr int TELE_TX = 11;
static constexpr int TELE_RX = 12;
static constexpr int TELE_RTS = A5;
static constexpr int TELE_CTS = A4;

static Uart TeleSerial(&sercom1, TELE_RX, TELE_TX, SERCOM_RX_PAD_3, UART_TX_PAD_0, TELE_RTS, TELE_CTS);

struct Coordinate {
   float lat, lon, alt; // degree, degree, meters
};

static TeleMode mode = TELE_MODE_RADIO;
static Time lastReceived = {0};
static Coordinate coord;
FlashStorage(coordStorage, Coordinate); // macro defining coordStorage

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
   
   // Restore saved coordinate
   tele_load();
}

void tele_load() {
   coordStorage.read(&coord);
}

void tele_save() {
   coordStorage.write(coord);
}

// get gps from telemetry
void tele_update() {
   bool received = false;
   switch(mode) {
      case TELE_MODE_RADIO:
         {
            char buff[GPS_MSG_LEN] = {GPS_MSG_HEADER};
            if(!Serial.findUntil(buff, 1, nullptr, 0)) {
               break;
            }

            if(Serial.readBytes(buff+1, GPS_MSG_LEN-1) < GPS_MSG_LEN-1) {
               break;
            }

            uint8_t latd, latm, latdm, latdir, lond, lonm, londm, londir;
            if(!expand_gps_message(&latd, &latm, &latdm, &latdir, &lond, &lonm, &londm, &londir, buff)) {
               break;
            }

            coord.lat = latd + (float) latm / 60 + (float) latdm / 600000;
            coord.lon = lond + (float) lonm / 60 + (float) londm / 600000;
            coord.alt = 0;

            if(latdir == 'S') coord.lat = -coord.lat;
            if(londir == 'W') coord.lon = -coord.lon;

            received = true;

            break;
         }
      case TELE_MODE_SERIAL:
         while(Serial.available()) {
            float num = Serial.parseFloat(SKIP_WHITESPACE);
            switch(Serial.read()) {
               case 'N':
                  coord.lat = num;
                  break;
               case 'S':
                  coord.lat = -num;
                  break;
               case 'E':
                  coord.lon = num;
                  break;
               case 'W':
                  coord.lon = -num;
                  break;
               case 'M':
                  coord.alt = num;
                  break;
               default:
                  break;
            }
            received = true;
         }
         break;
   }

   if(received) {
      gps_time(lastReceived);
      digitalWrite(LED, HIGH);
   } else {
      digitalWrite(LED, LOW);
   }

   // display
   int x = DISPLAY_W - 220;
   int y = DISPLAY_H - 100;

   display.setCursor(x, y);

   // display time
   switch(mode) {
      case TELE_MODE_RADIO:
         display.print("Teletry");
         break;
      case TELE_MODE_SERIAL:
         display.print("Serial");
         break;
   }

   y += LINE_H;
   display.setCursor(x, y);

   if(lastReceived.hour < 10) display.print('0');
   display.print(lastReceived.hour, DEC); display.print(':');
   if(lastReceived.minute < 10) display.print('0');
   display.print(lastReceived.minute, DEC); display.print(':');
   if(lastReceived.second < 10) display.print('0');
   display.print(lastReceived.second, DEC);

   y += LINE_H;
   display.setCursor(x, y);

   char buff[24];
   int deg, min;
   float sec;

   parsedeg(fabs(coord.lat), deg, min, sec);
   snprintf(buff, sizeof(buff), "LAT% 3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, coord.lat >= 0 ? 'N' : 'S');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   parsedeg(fabs(coord.lon), deg, min, sec);
   snprintf(buff, sizeof(buff), "LON% 3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, coord.lon >= 0 ? 'E' : 'W');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   snprintf(buff, sizeof(buff), "ALT % 4.2fM", coord.alt);
   display.print(buff);
}

void tele_coord(float &lat, float &lon) {
   lat = coord.lat / 360 * TWO_PI;
   lon = coord.lon / 360 * TWO_PI;
}

void tele_set_mode(TeleMode _mode) {
   mode = _mode;
}

TeleMode tele_get_mode() {
   return mode;
}
