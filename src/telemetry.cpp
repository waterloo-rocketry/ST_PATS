#include <Arduino.h>
#include <wiring_private.h> // pinPeripheral
#include <FlashStorage.h>

#include "telemetry.h"
#include "display.h"
#include "gps.h"
#include "crc8.h"

static constexpr int TELE_TX = 11;
static constexpr int TELE_RX = 12;
static constexpr int TELE_RTS = A4;
static constexpr int TELE_CTS = A5;

static constexpr int BUFF_SIZE = 15;
static constexpr int GPS_LAT_ID = 0x016;
static constexpr int GPS_LON_ID = 0x017;
static constexpr int GPS_ALT_ID = 0x018;
static constexpr int GPS_INFO_ID = 0x019;

/* enum MessageType {
   GPS_LATITUDE = 0x6E0,//need to check if these should match GPS-LAT_ID
   GPS_LONGITUDE = 0x700,
   GPS_ALTITUDE = 0x720,
}; */

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

// get gps from radio
static bool tele_recv_radio() {
   static enum {
      WAITING,
      SIZE,
      SID,
      DATA,
      CHECKSUM
   } state = WAITING;

   static struct {
      uint8_t data_len;
      uint8_t len;
      uint32_t sid;
      uint8_t data[8];
      uint8_t checksum;
   } msg = {0};

   // circular buffer for back tracking when header is out of sync
   static uint8_t buff[BUFF_SIZE] = {0};
   static int head = 0, end = 0, now = 0;

   // keep track of these so no need to calculate from head, end, now
   static int count = 0;
   bool received = false;

   // handle telemetry radio
   while(TeleSerial.available()) {
      // write to buffer
      buff[end] = TeleSerial.read();
      Serial.write(buff[end]);
      end = (end + 1) % sizeof(buff);
      if(end == head) {
         // overflow, malformed message (too large), reset
         state = WAITING;
      }

      // process buffer
      // normally, just the last byte that was read is processed
      // but in case of mismatched checksum, now is rolled back to the next
      // header byte and bytes in buffer is re-parsed.
      while(now != end) {
         uint8_t b = buff[now];
         switch(state) {
            case WAITING:
               if(b == 0x02) { // header byte
                  msg.sid = 0;
                  msg.data_len = 0;
                  msg.len=0;
                  msg.checksum = 0;
                  head = now;
                  count = 0;
                  state = SIZE;
               }
               break;

            case SIZE:
               msg.len = b & 0x0F;
               msg.data_len=msg.len-7;
               state = SID;
               break;

            case SID:
               msg.sid = (msg.sid << 8) | b;
               if(count >= 5) {
                  state = DATA;
               }
               break;

            case DATA:
               msg.data[count-6] = b;
               if(count >= msg.len - 2) {
                  state = CHECKSUM;
               }
               break;

            case CHECKSUM:
               if(msg.checksum != b) {
                  // mismatched checksym
                  // find the next header in buffer and rollback to that position
                  // if no header is found, then now = end and loop exits
                  now = head;
                  do {
                     now = (now + 1) % sizeof(buff);
                  } while(now != end && buff[now] != 0x02);
               } else {
                  // process the parsed message
                  switch((msg.sid>>18) & 0x1FF) { //extracts message type from bits 18-26
                     case GPS_LAT_ID:
                        if(msg.data_len < 7) break;
                        coords[TELE_MODE_RADIO].lat = (msg.data[2]
                           + (float) msg.data[3] / 60
                           + (float) (msg.data[4] << 8 | msg.data[5]) / 600000) / 360 * TWO_PI;
                        if(msg.data[6] == 'S') coords[TELE_MODE_RADIO].lat = -coords[TELE_MODE_RADIO].lat;
                        received = true;
                        break;

                     case GPS_LON_ID:
                        if(msg.data_len < 7) break;
                        coords[TELE_MODE_RADIO].lon = (msg.data[2]
                           + (float) msg.data[3] / 60
                           + (float) (msg.data[4] << 8 | msg.data[5]) / 600000) / 360 * TWO_PI;
                        if(msg.data[6] == 'W') coords[TELE_MODE_RADIO].lon = -coords[TELE_MODE_RADIO].lon;
                        received = true;
                        break;

                     case GPS_ALT_ID:
                        if(msg.data_len < 6) break;
                        coords[TELE_MODE_RADIO].alt = (float)((uint32_t) msg.data[2]<<24 | msg.data[3] << 16 | msg.data[4]<<8 | msg.data[5])/ 100;
                        received = true;
                        break;

                     case GPS_INFO_ID:
                        numSats = msg.data[2];
                        break;
                  }
               }

               state = WAITING;
               continue; // skip the rest of processing
         }

         msg.checksum = crc8_checksum(&b, 1, msg.checksum);
         now = (now + 1) % sizeof(buff);
         count++;
      }
   }

   return received;
}

//Self-Comment for Pranav - Made change required up till here
// handle USB serial
static bool tele_recv_usb() {
   bool received = false;
   while(Serial.available()) {
      TeleSerial.write(Serial.read());
      continue;
      float num = Serial.parseFloat(SKIP_ALL);
      switch(Serial.read()) {
         case 'N': case 'n':
            coords[TELE_MODE_SERIAL].lat = num / 360 * TWO_PI;
            break;
         case 'S': case 's':
            coords[TELE_MODE_SERIAL].lat = -num / 360 * TWO_PI;
            break;
         case 'E': case 'e':
            coords[TELE_MODE_SERIAL].lon = num / 360 * TWO_PI;
            break;
         case 'W': case 'w':
            coords[TELE_MODE_SERIAL].lon = -num / 360 * TWO_PI;
            break;
         case 'M': case 'm':
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
   return received;
}

static void tele_display() {
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

void tele_update() {
   bool received = false;
   received |= tele_recv_radio();
   received |= tele_recv_usb();

   if(received) {
      gps_time(lastReceived);
      digitalWrite(LED, HIGH);
   } else {
      digitalWrite(LED, LOW);
   }

   tele_display();
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
