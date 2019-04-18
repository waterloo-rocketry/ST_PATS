#include <Wire.h>
#include "display.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial GPS_serial(10, 9); //RX, TX

Adafruit_GPS GPS(&GPS_serial);
Display display;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("ST PATS"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }

  display.begin();
  display.set_background();
  
  GPS.begin(9600);
  // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_2HZ);   // 1 Hz update rate

  useInterrupt();
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
}

void useInterrupt() {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
}

void loop(void) 
{
  
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float Pi = 3.14159;
  
  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
  
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  display.draw_arrow(heading);
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  
  char c = GPS.read();
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
  
    if (GPS.parse(GPS.lastNMEA())){
    
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.println((int)GPS.fix);
    Serial.print("quality: "); Serial.println((int)GPS.fixquality); 
 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 5);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 5);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

    }
    }

  }
}
