#include "BigRedBee.h"

double BigRedBee::minutes_to_decimal (double minutes){     // converts gps hours and minutes to decimal degrees
      int degrees = minutes/100;
      minutes = minutes - (degrees*100);
      return ((double)degrees + minutes/60);
    }

BigRedBee::BigRedBee(HardwareSerial *serial_to_use){
  time_of_last_msg = 0;
  radio_serial = serial_to_use;
  longitude = 0, latitude = 0;
  time = 0, altitude = 0, num_sats = 0;
  status = '-';
  for(int index = 0; index < 4; index++){
    hex_voltage[index] = 0;
  }
  Vbat = 0; 
  HDOP = 0, VDOP = 0;
}

void BigRedBee::begin(int baud){
  radio_serial->begin(baud);
}

/* Example Sentances for reference:
 *  $BRBTX,ID0473,-,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,00,,,,,,,,,,,,,,,,,,217*66
 *  $BRBTX,ID0473,-,221238,,,,,,,,,,,,,,,,,,,,,,,,00,,,,,,,,,,,,,,,,,,215*6C
 *  $BRBTX,ID0473,^,221338,4328.4024N,08032.4483W,09,1.07,001095,1.86,213*1D
 *  $BRBTX,ID0382,-,222038,4328.3936N,08032.4245W,00,22.6400,347,1.00,208*62
 *  $BRBTX,ID0382,*,222313,4328.3967N,08032.4352W,10,0.95,000334,1.42,208*61
 */

void BigRedBee::parse_data(){
  unsigned int start_time = millis();
  if(radio_serial->available() >= 72){
    if(radio_serial->find('$')){     //find the begining of the next string
    if(radio_serial->readStringUntil(',') == "BRBTX"){  // confirm it is a bigRedBee GPS sentance
      radio_serial->find(',');     //skip the ID because I don't care
      while(radio_serial->available() == 0){
        if(millis() - start_time > TIMEOUT)
          return;
        }     //wait for data
      status = radio_serial->read();     //read our status
      radio_serial->find(',');      //ignore a comma
      while(radio_serial->available() == 0){
        if(millis() - start_time > TIMEOUT)
          return;
      }
      if(radio_serial->peek() != ','){          // check to see if there is a valid GPS time to parse
        time = radio_serial->parseInt();      //parse the time
      }
      radio_serial->find(',');      //ignore a comma
      while(radio_serial->available() == 0){
        if(millis() - start_time > TIMEOUT)
          return;
      }
      if(radio_serial->peek() != ','){
        latitude = minutes_to_decimal(radio_serial->parseFloat());     //if we have a longitude and latitude parse it
        while(radio_serial->available() == 0){
          if(millis() - start_time > TIMEOUT)
          return;
        }
        if(radio_serial->read() == 'S')
          latitude = -latitude;
        longitude = minutes_to_decimal(radio_serial->parseFloat());
        while(radio_serial->available() == 0){
          if(millis() - start_time > TIMEOUT)
            return;
        }
        if(radio_serial->read() == 'W')
          longitude = -longitude;
      }
      num_sats = radio_serial->parseInt();      //parse the number of satalites
      radio_serial->find(',');
      while(radio_serial->available() == 0){
        if(millis() - start_time > TIMEOUT)
          return;
      }
      if(radio_serial->peek() != ','){
        HDOP = radio_serial->parseFloat();      //parse HDOP
        altitude = radio_serial->parseInt();      //parse height (in feet)
        VDOP = radio_serial->parseFloat();      //parse HDOP
      }
      while(radio_serial->available() == 0){
        if(millis() - start_time > TIMEOUT)
          return;
      }
      do{
        //if(radio_serial->peek() == ',')
          radio_serial->read();
          while(radio_serial->available() == 0){
            if(millis() - start_time > TIMEOUT)
              return;
          }
      }while(radio_serial->peek() == ',');
      radio_serial->readBytesUntil('*', hex_voltage, 3);
      sscanf(hex_voltage, "%x", &Vbat);
      
      // for debuging
      #ifdef SERIAL_DEBUGING
      Serial.print("time: ");
      Serial.println(time);
      Serial.print("num_sats: ");
      Serial.println(num_sats);
      Serial.print("longitude: ");
      Serial.println(longitude, 4);
      Serial.print("latitude: ");
      Serial.println(latitude, 4);
      Serial.print("altitude: ");
      Serial.println(altitude);
      Serial.print("HDOP: ");
      Serial.println(HDOP);
      Serial.print("VDOP: ");
      Serial.println(VDOP);
      Serial.print("Vbat: ");
      Serial.println(Vbat);
      Serial.print("hex_voltage: ");
      Serial.println(hex_voltage);
      Serial.println("///////////////////////");
      #endif
      
    time_of_last_msg = millis();
    }
    }
    }
    }

long BigRedBee::time_since_last_msg(){
  return millis() - time_of_last_msg;
}
