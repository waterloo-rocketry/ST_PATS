#include <stdio.h>
#include <Adafruit_GPS.h>

#include "display.h"

#define GPSSerial Serial1

static Adafruit_GPS gps(&GPSSerial);

static void parsedeg(int32_t degFixed, int &deg, int &min, int &sec, int &msec) {
   deg = degFixed / 10000000;
   degFixed %= 10000000;
   min = degFixed * 60 / 10000000;
   degFixed = degFixed * 60 % 10000000;
   sec = degFixed * 60 / 10000000;
   degFixed = degFixed * 60 % 10000000;
   msec = degFixed / 10000;
}

void gps_init() {
   gps.begin(9600);
   gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
   gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
   gps.sendCommand(PGCMD_ANTENNA);
}

void gps_update() {
   // read GPS
   while(gps.available()) {
      gps.read();
      if(gps.newNMEAreceived()) {
         gps.parse(gps.lastNMEA());
      }
   }

   // display
   int x = DISPLAY_W - 200;
   int y = 10;

   display.setCursor(x, y);

   display.print("Local GPS");
   y += LINE_H;
   display.setCursor(x, y);

   if(gps.hour < 10) display.print('0');
   display.print(gps.hour, DEC); display.print(':');
   if(gps.minute < 10) display.print('0');
   display.print(gps.minute, DEC); display.print(':');
   if(gps.seconds < 10) display.print('0');
   display.print(gps.seconds, DEC);

   y += LINE_H;
   display.setCursor(x, y);

   if(gps.fix) {
      char buff[20];
      int deg, min, sec, msec;

      parsedeg(fabs(gps.latitude_fixed), deg, min, sec, msec);
      sprintf(buff, "LAT% 3d\xF8%02d'%02d.%03d\"%c", deg, min, sec, msec, gps.lat);
      display.print(buff);

      y += LINE_H;
      display.setCursor(x, y);

      parsedeg(fabs(gps.longitude_fixed), deg, min, sec, msec);
      sprintf(buff, "LON% 3d\xF8%02d'%02d.%03d\"%c", deg, min, sec, msec, gps.lon);
      display.print(buff);

      y += LINE_H;
      display.setCursor(x, y);

      sprintf(buff, "ALT % 4.2fM", gps.altitude);
      display.print(buff);
   } else {
      display.print("NO FIX");
   }
}

void gps_coord(float &lat, float &lon) {
   lat = gps.latitude / 360 * TWO_PI;
   lon = gps.longitude / 360 * TWO_PI;
}

float gps_magvariation() {
   return gps.magvariation / 360 * TWO_PI;
}
