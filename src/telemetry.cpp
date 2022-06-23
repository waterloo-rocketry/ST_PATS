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
static constexpr int GPS_LAT_ID = 0x6E0;
static constexpr int GPS_LON_ID = 0x700;
static constexpr int GPS_ALT_ID = 0x720;
static constexpr int GPS_INFO_ID = 0x740;

enum MessageType {
   GPS_LATITUDE = 0x6E0,
   GPS_LONGITUDE = 0x700,
   GPS_ALTITUDE = 0x720,
};

static Uart TeleSerial(&sercom1, TELE_RX, TELE_TX, SERCOM_RX_PAD_3, UART_TX_PAD_0, TELE_RTS, TELE_CTS);

struct Coordinate {
   float lat, lon, alt; // rad, rad, meters
};

static TeleMode mode = TELE_MODE_RADIO;
static Time lastReceived = {0};
static int numSats = 0;
static Coordinate coords[TELE_MODE_MAX];

// macro defining coordStorage*
FlashStorage(coordStorage0, Coordinate);
FlashStorage(coordStorage1, Coordinate);
FlashStorage(coordStorage2, Coordinate);

FlashStorageClass<Coordinate> coordStorages[TELE_MODE_MAX] = {
   coordStorage0, coordStorage1, coordStorage2,
};

static void to_degs(float rad, int &deg, float &min) {
   float degFloat = rad / TWO_PI * 360;
   deg = (int) degFloat;
   min = (degFloat - deg) * 60;
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
   for(TeleMode m = TELE_MODE_RADIO; m < TELE_MODE_MAX; m = TeleMode(int(m)+1)) {
      tele_load(m);
   }
}

void tele_load(TeleMode mode) {
   coordStorages[mode].read(&coords[mode]);
}

void tele_save(TeleMode mode) {
   switch(mode) {
      case TELE_MODE_HOME:
         if(!gps_fixed()) return;
         gps_coord(coords[mode].lat, coords[mode].lon, coords[mode].alt);
         coordStorages[mode].write(coords[mode]);
         break;
      default:
         coordStorages[mode].write(coords[mode]);
         break;
   }
}

// get gps from telemetry
void tele_update() {
   bool received = false;

   // handle telemetry radio
   while(TeleSerial.available() >= GPS_LEN_MAX) {
      char buff[GPS_LEN_MAX+1] = {0}; // have a trailing zero

      TeleSerial.readBytesUntil('\n', buff, sizeof(buff));

      int sid = (int) strtol(buff+1, nullptr, 16);
      int type = sid & 0x7e0;
      int data[8];
      int len;

      switch(type) {
         case GPS_LAT_ID:
         case GPS_LON_ID:
         case GPS_ALT_ID:
         case GPS_INFO_ID:
            len = sscanf(buff, "$%*3x:%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x", data, data+1, data+2, data+3, data+4, data+5, data+6, data+7);
            break;
         default:
            continue;
      }

      switch(type) {
         case GPS_LAT_ID:
            if(len < 8) break;
            coords[TELE_MODE_RADIO].lat = (data[3] + (float) data[4] / 60 + (float) (data[5] << 8 | data[6]) / 600000) / 360 * TWO_PI;
            if(data[7] == 'S') coords[TELE_MODE_RADIO].lat = -coords[TELE_MODE_RADIO].lat;
            received = true;
            break;
         case GPS_LON_ID:
            if(len < 8) break;
            coords[TELE_MODE_RADIO].lon = (data[3] + (float) data[4] / 60 + (float) (data[5] << 8 | data[6]) / 600000) / 360 * TWO_PI;
            if(data[7] == 'W') coords[TELE_MODE_RADIO].lon = -coords[TELE_MODE_RADIO].lon;
            received = true;
            break;
         case GPS_ALT_ID:
            if(len < 5) break;
            coords[TELE_MODE_RADIO].alt = (data[3] << 8 | data[4]) + (float) data[5] / 100;
            received = true;
            break;
         case GPS_INFO_ID:
            numSats = data[3];
            break;
      }
   }

   // handle USB serial
   while(Serial.available()) {
      float num = Serial.parseFloat(SKIP_ALL);
      switch(Serial.read()) {
         case 'N':
            coords[TELE_MODE_SERIAL].lat = num / 360 * TWO_PI;
            break;
         case 'S':
            coords[TELE_MODE_SERIAL].lat = -num / 360 * TWO_PI;
            break;
         case 'E':
            coords[TELE_MODE_SERIAL].lon = num / 360 * TWO_PI;
            break;
         case 'W':
            coords[TELE_MODE_SERIAL].lon = -num / 360 * TWO_PI;
            break;
         case 'M':
            coords[TELE_MODE_SERIAL].alt = num;
            break;
         case '#':
            numSats = num;
            break;
         default:
            break;
      }
      received = true;
   }

   if(received) {
      gps_time(lastReceived);
      digitalWrite(LED, HIGH);
   } else {
      digitalWrite(LED, LOW);
   }

   // display
   int x = DISPLAY_W - 235;
   int y = DISPLAY_H - 118;

   display.setCursor(x, y);

   // display time
   switch(mode) {
      case TELE_MODE_RADIO:
         display.print("Telemetry");
         break;
      case TELE_MODE_SERIAL:
         display.print("Serial");
         break;
      case TELE_MODE_HOME:
         display.print("Home");
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
   int deg;
   float min;

   to_degs(fabs(coords[mode].lat), deg, min);
   snprintf(buff, sizeof(buff), "LAT %3d\xF8%06.3f\'%c", deg % 1000, min, coords[mode].lat >= 0 ? 'N' : 'S');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   to_degs(fabs(coords[mode].lon), deg, min);
   snprintf(buff, sizeof(buff), "LON %3d\xF8%06.3f\'%c", deg % 1000, min, coords[mode].lon >= 0 ? 'E' : 'W');
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   snprintf(buff, sizeof(buff), "ALT  %4.2fM", coords[mode].alt);
   display.print(buff);

   y += LINE_H;
   display.setCursor(x, y);

   snprintf(buff, sizeof(buff), "#SAT %d", numSats);
   display.print(buff);
}

void tele_coord(float &lat, float &lon) {
   lat = coords[mode].lat;
   lon = coords[mode].lon;
}

void tele_set_mode(TeleMode _mode) {
   mode = _mode;
}

TeleMode tele_get_mode() {
   return mode;
}
