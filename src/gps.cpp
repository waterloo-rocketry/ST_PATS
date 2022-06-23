#include <stdio.h>
#include <Adafruit_GPS.h>

#include "gps.h"
#include "display.h"

#define GPSSerial Serial1

static Adafruit_GPS gps(&GPSSerial);

static void parsedeg(int32_t degFixed, int &deg, float &min) {
   deg = degFixed / 10000000;
   degFixed = degFixed % 10000000 * 60;
   min = (float) degFixed / 10000000;
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
   int x = DISPLAY_W - 235;
   int y = 8;

   display.setCursor(x, y);

   display.print("Local");
   y += LINE_H;
   display.setCursor(x, y);

   display.print("TIME ");
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
      int deg;
      float min;

      parsedeg(fabs(gps.latitude_fixed), deg, min);
      snprintf(buff, sizeof(buff), "LAT %3d\xF8%06.3f\'%c", deg, min, gps.lat);
      display.print(buff);

      y += LINE_H;
      display.setCursor(x, y);

      parsedeg(fabs(gps.longitude_fixed), deg, min);
      snprintf(buff, sizeof(buff), "LON %3d\xF8%06.3f\'%c", deg, min, gps.lon);
      display.print(buff);

      y += LINE_H;
      display.setCursor(x, y);

      snprintf(buff, sizeof(buff), "ALT  %4.2fM", gps.altitude);
      display.print(buff);

      y += LINE_H;
      display.setCursor(x, y);

      snprintf(buff, sizeof(buff), "#SAT %d", gps.satellites);
      display.print(buff);
   } else {
      display.print("NO FIX");
   }
}

void gps_coord(float &lat, float &lon, float &alt) {
   lat = gps.latitude_fixed / 3600000000.0 * TWO_PI;
   lon = gps.longitude_fixed / 3600000000.0 * TWO_PI;
   alt = gps.altitude;
}

float gps_magvariation() {
   return gps.magvariation / 360 * TWO_PI;
}

bool gps_fixed() {
   return gps.fix;
}

void gps_time(Time &time) {
   time.hour = gps.hour;
   time.minute = gps.minute;
   time.second = gps.seconds;
}
