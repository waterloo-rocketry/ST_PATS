#include <Arduino.h>
#include <wiring_private.h> // pinPeripheral
#include <FlashStorage.h>

#include "telemetry.h"
#include "display.h"
#include "gps.h"

static constexpr int TELE_TX = 11;
static constexpr int TELE_RX = 12;
static constexpr int TELE_RTS = A4;
static constexpr int TELE_CTS = A5;

static constexpr int GPS_LEN_MAX = 28;
static constexpr int GPS_BOARD_ID = 0x0D;
static constexpr int GPS_LAT_ID = 0x6E0;
static constexpr int GPS_LON_ID = 0x700;
static constexpr int GPS_ALT_ID = 0x720;

enum MessageType {
   GPS_LATITUDE = 0x6E0,
   GPS_LONGITUDE = 0x700,
   GPS_ALTITUDE = 0x720,
};

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
   TeleSerial.begin(115200);
   TeleSerial.setTimeout(200);
   pinPeripheral(TELE_RX, PIO_SERCOM);
   pinPeripheral(TELE_TX, PIO_SERCOM);

   // USB Serial
   Serial.begin(9600);
   Serial.setTimeout(200);
   
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
            while(TeleSerial.available() >= GPS_LEN_MAX) {
               char buff[GPS_LEN_MAX+1] = {0}; // have a trailing zero

               TeleSerial.readBytesUntil('\n', buff, sizeof(buff));

               int sid, data[8];
               int argc = sscanf(buff, "$%3x:%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x", &sid, data, data+1, data+2, data+3, data+4, data+5, data+6, data+7);
               int type = sid & 0x7e0;
               int board = sid & 0x1f;

               if(board != GPS_BOARD_ID) {
                  continue;
               }

               switch(type) {
                  case GPS_LAT_ID:
                     if(argc <= 8) break;
                     coord.lat = data[3] + (float) data[4] / 60 + (float) (data[5] << 8 | data[6]) / 600000;
                     if(data[7] == 'S') coord.lat = -coord.lat;
                     received = true;
                     break;
                  case GPS_LON_ID:
                     if(argc <= 8) break;
                     coord.lon = data[3] + (float) data[4] / 60 + (float) (data[5] << 8 | data[6]) / 600000;
                     if(data[7] == 'W') coord.lon = -coord.lon;
                     received = true;
                     break;
                  case GPS_ALT_ID:
                     if(argc <= 7) break;
                     coord.alt = (data[3] << 8 | data[4]) + (float) data[5] / 100;
                     received = true;
                     break;
               }
            }
         }
      case TELE_MODE_SERIAL:
         while(Serial.available()) {
            float num = Serial.parseFloat(SKIP_ALL);
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
   int x = DISPLAY_W - 235;
   int y = DISPLAY_H - 100;

   display.setCursor(x, y);

   // display time
   switch(mode) {
      case TELE_MODE_RADIO:
         display.print("Telemetry");
         break;
      case TELE_MODE_SERIAL:
         display.print("Serial");
         break;
      default:
         display.print("Unknown");
         break;
   }

   y += LINE_H;
   display.setCursor(x, y);

   display.print("LAST ");
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
   snprintf(buff, sizeof(buff), "LAT %3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, coord.lat >= 0 ? 'N' : 'S');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   parsedeg(fabs(coord.lon), deg, min, sec);
   snprintf(buff, sizeof(buff), "LON %3d\xF8%02d'%06.3f\"%c", deg % 1000, min, sec, coord.lon >= 0 ? 'E' : 'W');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   snprintf(buff, sizeof(buff), "ALT  %4.2fM", coord.alt);
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
