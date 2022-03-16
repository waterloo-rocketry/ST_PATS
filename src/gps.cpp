#include <Adafruit_GPS.h>

#include "config.h"
#include "display.h"

static Adafruit_GPS gps(&GPSSerial);

void gps_init() {
   gps.begin(9600);
   gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
   gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
   gps.sendCommand(PGCMD_ANTENNA);
}

void gps_update() {
   display.setCursor(0, 120);

   while(gps.available()) {
      gps.read();
      if(gps.newNMEAreceived() && gps.parse(gps.lastNMEA())) {
         display.print("\nTime: ");
         if(gps.hour < 10) { display.print('0'); }
         display.print(gps.hour, DEC); display.print(':');
         if(gps.minute < 10) { display.print('0'); }
         display.print(gps.minute, DEC); display.print(':');
         if(gps.seconds < 10) { display.print('0'); }
         display.print(gps.seconds, DEC); display.print('.');
         if(gps.milliseconds < 10) {
            display.print("00");
         } else if (gps.milliseconds > 9 && gps.milliseconds < 100) {
            display.print("0");
         }
         display.println(gps.milliseconds);
         break;
      }
   }
}
