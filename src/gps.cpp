#include <stdio.h>
#include <Adafruit_GPS.h>

#include "config.h"
#include "display.h"

static Adafruit_GPS gps(&GPSSerial);

static void parse_deg(int32_t degFixed, int &deg, int &min, int &sec, int &msec) {
   deg = degFixed / 10000000;
   degFixed %= 10000000;
   min = degFixed / 3600000;
   degFixed %= 3600000;
   sec = degFixed / 60000;
   degFixed %= 60000;
   msec = degFixed / 1000;
}

void gps_init() {
   gps.begin(9600);
   gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
   gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
   gps.sendCommand(PGCMD_ANTENNA);
}

void gps_update() {
   while(gps.available()) {
      gps.read();
      if(gps.newNMEAreceived()) {
         gps.parse(gps.lastNMEA());
      }
   }

   static const int lineHeight = 18;
   int x = DISPLAY_W - 200;
   int y = 10;

   display.setCursor(x, y);

   // display time
   display.print("Local GPS");
   y += lineHeight;
   display.setCursor(x, y);

   if(gps.hour < 10) display.print('0');
   display.print(gps.hour, DEC); display.print(':');
   if(gps.minute < 10) display.print('0');
   display.print(gps.minute, DEC); display.print(':');
   if(gps.seconds < 10) display.print('0');
   display.print(gps.seconds, DEC);

   y += lineHeight;
   display.setCursor(x, y);

   if(gps.fix) {
      char buff[24];
      int deg, min, sec, msec;

      parse_deg(fabs(gps.latitude_fixed), deg, min, sec, msec);
      sprintf(buff, "LAT% 3d\xF8%02d'%02d\"%03d", deg, min, sec, msec);
      display.print(buff);

      y += lineHeight;
      display.setCursor(x, y);

      parse_deg(fabs(gps.longitude_fixed), deg, min, sec, msec);
      sprintf(buff, "LON% 3d\xF8%02d'%02d\"%03d", deg, min, sec, msec);
      display.print(buff);

      y += lineHeight;
      display.setCursor(x, y);

      sprintf(buff, "ALT % 4.2fM", gps.altitude);
      display.print(buff);
   } else {
      display.print("NO FIX");
   }
}
