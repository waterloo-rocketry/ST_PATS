/*
 * IMPORTANT: In addition to the required libraries the serial RX buffer needs to be set to 256(might need to be 4096 if dealing with BigRedBee in debug mode bytes instead of 64
 * you should find this number in HardwareSerial.h at somewhere like this:
 * ...\ArduinoData\packages\STM32\hardware\stm32\1.4.0\cores\arduino
 */

#include <Wire.h>
#include "display.h"
#include "BigRedBee.h"
#include "RocketCAN.h"
#include "compass.h"
#include "navigation.h"
#include "EEPROM_storage.h"
#include "sd_handler.h"
#include "Adafruit_GPS_modded.h"

#define TIME_BETWEEN_UPDATES 1000 // in ms
#define TIME_BETWEEN_SCREEN_RESET 60000*15 // in ms

#define RED_BUTTON PB11
#define GREEN_BUTTON PB10
#define BLUE_BUTTON PE15

#define RED_LED PE0
#define GREEN_LED PB0
#define BLUE_LED PA0

static enum {
  BIG_RED_BEE,
  ROCKET_CAN,
  LOCAL
}nav_GPS;

unsigned long time_of_last_update = 0;
unsigned long time_of_last_screen_reset = 0;

EEPROM_data EEPROM_nav_data;


// Create objects for all our sensors:

Display display;

Compass compass;

//BigRedBee serial line definition
HardwareSerial Serial5(PD2, PC12);
BigRedBee brb_GPS(&Serial5);

//RocketCAN serial line definition
HardwareSerial Serial4(PC11, PC10);
RocketCAN CAN_GPS(&Serial4);

//Adafruit_GPS serial line definition
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
  CAN_GPS.read();
}


void setup(void) 
{
  EEPROM_nav_data.nav = false;
  EEPROM_nav_data.latitude = 0;
  EEPROM_nav_data.longitude = 0;
  display.power_up();
  display.begin();
  display.set_background();
  
  Serial.begin(9600);
  Serial.println("ST PATS"); Serial.println("");
  
  /* lets set up our sensors */
  local_GPS.begin(9600);
  local_GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  local_GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  brb_GPS.begin(9600);
  CAN_GPS.begin(9600);
  compass.begin();
  sd_init();

  /* lets set up our interupt timer for reading all the UART data */
  Timer_Handler.timer = TIM4;
  /* Timer set to 1ms, */
  TimerHandleInit(&Timer_Handler, 1, (uint32_t)(getTimerClkFreq(TIM4) / 1000));
  attachIntHandle(&Timer_Handler, read_gps_data);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);

}

void loop(void) 
{
  if(millis() - time_of_last_update > TIME_BETWEEN_UPDATES){
    time_of_last_update = millis();

    //Parse the BigRedBee input
    for(int index = 0; index < 20; index++){
      brb_GPS.parse_data();
    }
    brb_GPS.log_to_sd();

    CAN_GPS.log_to_sd();

    flush();

    
    compass.read();
    //display.draw_arrow(compass.get_heading());

  if(EEPROM_nav_data.nav){
    get_GPS_location(EEPROM_nav_data.latitude, EEPROM_nav_data.longitude, nav_GPS);
    display.draw_arrow(target_heading(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees,
    EEPROM_nav_data.latitude, EEPROM_nav_data.longitude));
  }
  else {
    if(nav_GPS == BIG_RED_BEE){
      display.draw_arrow(target_heading(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees,
      brb_GPS.latitude, brb_GPS.longitude));
    }
    else if(nav_GPS == ROCKET_CAN){
      display.draw_arrow(target_heading(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees,
      CAN_GPS.latitude, CAN_GPS.longitude));
    }
    else {
      display.draw_arrow(target_heading(0, 0, 1, 0));
    }
  }
    display.write_GPS("BRBee:", brb_GPS.time_since_last_msg()/1000.0, brb_GPS.latitude, brb_GPS.longitude, nav_GPS == BIG_RED_BEE);
    display.write_GPS("R_CAN:", CAN_GPS.time_since_last_msg()/1000.0, CAN_GPS.latitude, CAN_GPS.longitude, nav_GPS == ROCKET_CAN);

  if(EEPROM_nav_data.nav){
      display.write_GPS("Local:", 
      distance_to_target(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, EEPROM_nav_data.latitude, EEPROM_nav_data.longitude), 
      local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, nav_GPS == LOCAL);
  }

  else {
    if(nav_GPS == BIG_RED_BEE){
      display.write_GPS("Local:", distance_to_target(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees,
      brb_GPS.latitude, brb_GPS.longitude), local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, nav_GPS == LOCAL);
    }
    else if(nav_GPS == ROCKET_CAN){
      display.write_GPS("Local:", distance_to_target(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees,
      CAN_GPS.latitude, CAN_GPS.longitude), local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, nav_GPS == LOCAL);
    }
    else {
      display.write_GPS("Local:", distance_to_target(0, 0, 0, 0), local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, nav_GPS == LOCAL);
    }
  }

    if(EEPROM_nav_data.nav){
      display.write_GPS("EEPROM:", nav_GPS, EEPROM_nav_data.latitude, EEPROM_nav_data.longitude, true);
    }
    else{
      display.clear_GPS();
    }

    display.write_local_data(local_GPS.hour, local_GPS.minute, local_GPS.seconds, local_GPS.satellites);
  
  // if a sentence is received, we can check the checksum, parse it...
  if (local_GPS.newNMEAreceived()) {
    local_GPS.parse(local_GPS.lastNMEA());
  }
  local_GPS.log_to_sd();

  /*
   * Do stuff if buttons are pressed
   */
  if(digitalRead(RED_BUTTON) == HIGH){
    digitalWrite(RED_LED, HIGH);
  }
  else{
    digitalWrite(RED_LED, LOW);
    if(nav_GPS == LOCAL){
      nav_GPS = BIG_RED_BEE;
    }
    else if(nav_GPS == BIG_RED_BEE){
      nav_GPS = ROCKET_CAN;
    }
    else {
      nav_GPS = LOCAL;
    }
  }
  if(digitalRead(GREEN_BUTTON) == HIGH){
    digitalWrite(GREEN_LED, HIGH);
  }
  else{
    digitalWrite(GREEN_LED, LOW);
    if(nav_GPS == LOCAL){
      save_GPS_location(local_GPS.latitudeDegrees, local_GPS.longitudeDegrees, LOCAL);
    }
    else if(nav_GPS == BIG_RED_BEE){
      save_GPS_location(brb_GPS.latitude, brb_GPS.longitude, BIG_RED_BEE);
    }
    else {
      save_GPS_location(CAN_GPS.latitude, CAN_GPS.longitude, ROCKET_CAN);
    }
  }
  if(digitalRead(BLUE_BUTTON) == HIGH){
    digitalWrite(BLUE_LED, HIGH);
  }
  else{
    digitalWrite(BLUE_LED, LOW);
    EEPROM_nav_data.nav = !EEPROM_nav_data.nav;
  }
  }
  
  if(millis() - time_of_last_screen_reset > TIME_BETWEEN_SCREEN_RESET){
    time_of_last_screen_reset = millis();
    display.power_down();
    delay(50);
    display.power_up();
    display.begin();
    display.set_background();
    sd_init();
  }

}
