#include <Wire.h>
#include "display.h"
#include "BigRedBee.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define TIME_BETWEEN_UPDATES 1000 // in ms

unsigned long time_of_last_update = 0;

SoftwareSerial GPS_serial(10, 9); //RX, TX
Adafruit_GPS local_GPS(&GPS_serial);

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified compass = Adafruit_LSM303_Mag_Unified(12345);

Display display;

void setup(void) 
{
  
  delay(250);
  Serial.begin(9600);
  Serial.println("ST PATS"); Serial.println("");
  
  
  display.begin();
  display.set_background();
  
  local_GPS.begin(9600);
  // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  local_GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
  local_GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  /* Initialise the sensor */
  compass.begin();
  
  useInterrupt();
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = local_GPS.read();
}

void useInterrupt() {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
}

void loop(void) 
{
  if(millis() - time_of_last_update > TIME_BETWEEN_UPDATES){
    time_of_last_update = millis();
    sensors_event_t event; 
    compass.getEvent(&event);
  
    // Calculate the angle of the vector y,x
    float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / PI;
  
    // Normalize to 0-360
    if (heading < 0)
    {
     heading = 360 + heading;
    }
    
    display.draw_arrow(heading);
    Serial.print("Compass Heading: ");
    Serial.println(heading);

  //display.write_data(syst, gyro, accel, mag, 48.2382, 123.659);
  char c = local_GPS.read();
  
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

  }
  }
}
