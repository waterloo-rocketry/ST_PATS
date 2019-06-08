#include <Wire.h>
#include "display.h"
#include "BigRedBee.h"
#include "compass.h"
#include <Adafruit_GPS.h>
//#include <SoftwareSerial.h>

#define TIME_BETWEEN_UPDATES 1000 // in ms
#define TIME_BETWEEN_SCREEN_RESET 10000 // in ms

unsigned long time_of_last_update = 0;
unsigned long time_of_last_screen_reset = 0;

// Create objects for all our sensors:

Display display;

Compass compass;

HardwareSerial Serial5(PD2, PC12);
BigRedBee brb_GPS(&Serial5);

HardwareSerial Serial3(PC5, PC4);
Adafruit_GPS local_GPS(&Serial3);

/* timer stuff */
static stimer_t Timer_Handler;

/*
 * Function for parsing all the gps data once per millisecond.
 * unused is a pointer to stimer_t or something, whatever it is we just have it to make function definitions happy
 */
static void read_gps_data(stimer_t *unused){
  char c = local_GPS.read();
  brb_GPS.parse_data();
}


void setup(void) 
{
  delay(250);// weird things happen if we don't let things boot up before trying to talk to them
  
  Serial.begin(9600);
  Serial.println("ST PATS"); Serial.println("");

  display.begin();
  display.set_background();
  
  /* lets set up our sensors */
  local_GPS.begin(9600);
  local_GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  local_GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  brb_GPS.begin(9600);
  compass.begin();

  /* lets set up our interupt timer for reading all the UART data */
  Timer_Handler.timer = TIM4;
  /* Timer set to 1ms, */
  TimerHandleInit(&Timer_Handler, 1, (uint32_t)(getTimerClkFreq(TIM4) / 1000));
  attachIntHandle(&Timer_Handler, read_gps_data);

}
/*
SIGNAL(TIMER0_COMPA_vect) {
  char c = local_GPS.read();
}

void useInterrupt() {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
}
*/
void loop(void) 
{
  //Serial.print(c);
  if(millis() - time_of_last_update > TIME_BETWEEN_UPDATES){
    time_of_last_update = millis();

    compass.read();
    
    display.draw_arrow(compass.get_heading());
    Serial.print("Compass Heading: ");
    Serial.println(compass.get_heading());
    display.reset_text();
    display.write_GPS("Local:", local_GPS.seconds, local_GPS.latitudeDegrees, local_GPS.longitudeDegrees);
    display.write_GPS("BigRedBee:", brb_GPS.time, brb_GPS.latitude, brb_GPS.longitude);
  
  // if a sentence is received, we can check the checksum, parse it...
  if (local_GPS.newNMEAreceived()) {
  
    if (local_GPS.parse(local_GPS.lastNMEA())){
    
    Serial.print("\nTime: ");
    Serial.print(local_GPS.hour, DEC); Serial.print(':');
    Serial.print(local_GPS.minute, DEC); Serial.print(':');
    Serial.print(local_GPS.seconds, DEC); Serial.print('.');
    Serial.println(local_GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(local_GPS.day, DEC); Serial.print('/');
    Serial.print(local_GPS.month, DEC); Serial.print("/20");
    Serial.println(local_GPS.year, DEC);
    Serial.print("Fix: "); Serial.println((int)local_GPS.fix);
    Serial.print("quality: "); Serial.println((int)local_GPS.fixquality); 
 
    if (local_GPS.fix) {
      Serial.print("Location: ");
      Serial.print(local_GPS.latitude, 4); Serial.print(local_GPS.lat);
      Serial.print(", "); 
      Serial.print(local_GPS.longitude, 4); Serial.println(local_GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(local_GPS.latitudeDegrees, 5);
      Serial.print(", "); 
      Serial.println(local_GPS.longitudeDegrees, 5);
      
      Serial.print("Speed (knots): "); Serial.println(local_GPS.speed);
      Serial.print("Angle: "); Serial.println(local_GPS.angle);
      Serial.print("Altitude: "); Serial.println(local_GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)local_GPS.satellites);

    }
    }
    else{
      Serial.println ("oops I couldn't parse GPS");
    }

  }
  }
  if(millis() - time_of_last_screen_reset > TIME_BETWEEN_SCREEN_RESET){
    time_of_last_screen_reset = millis();
    display.begin();
    display.set_background();
  }
}
