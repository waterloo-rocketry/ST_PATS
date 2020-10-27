#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "display.h"

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void setup() {
  Serial.begin(9600);
  
  mag.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}
