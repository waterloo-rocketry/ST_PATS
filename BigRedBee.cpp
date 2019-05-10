#include "BigRedBee.h"

float BigRedBee::minutes_to_decimal (float minutes){     // converts gps hours and minutes to decimal degrees
      int degrees = minutes/100;
      minutes = minutes - (degrees*100);
      return ((float)degrees + minutes/60);
    }

BigRedBee::BigRedBee(){
  longditude = 0, latitude = 0;
  time = 0, altitude = 0, num_sats = 0;
  status = '-';
  Vbat = 0; 
  HDOP = 0, VDOP = 0;
}

void BigRedBee::begin(int baud){
  Serial1.begin(baud);
}

/* Example Sentances for reference:
 *  $BRBTX,ID0473,-,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,00,,,,,,,,,,,,,,,,,,217*66
 *  $BRBTX,ID0473,-,221238,,,,,,,,,,,,,,,,,,,,,,,,00,,,,,,,,,,,,,,,,,,215*6C
 *  $BRBTX,ID0473,^,221338,4328.4024N,08032.4483W,09,1.07,001095,1.86,213*1D
 *  $BRBTX,ID0382,-,222038,4328.3936N,08032.4245W,00,22.6400,347,1.00,208*62
 *  $BRBTX,ID0382,*,222313,4328.3967N,08032.4352W,10,0.95,000334,1.42,208*61
 */

void BigRedBee::parse_data(){
  if(Serial1.available() > 0){
    if(Serial1.find('$')){     //find the begining of the next string
      Serial1.find(',');      //skip the "BRBTX
      Serial1.find(',');     //skip the ID because I don't care
      while(Serial1.available() == 0){}     //wait for data
      status = Serial1.read();     //read our status
      Serial1.find(',');      //ignore a comma
      while(Serial1.available() == 0){}
      if(Serial1.peek() != ','){          // check to see if there is a valid GPS time to parse
        time = Serial1.parseInt();      //parse the time
      }
      Serial1.find(',');      //ignore a comma
      while(Serial1.available() == 0){}
      if(Serial1.peek() != ','){
        longditude = minutes_to_decimal(Serial1.parseFloat());     //if we have a longditude and latitude parse it
        while(Serial1.available() == 0){}
        if(Serial1.read() == 'S')
          longditude = -longditude;
        latitude = minutes_to_decimal(Serial1.parseFloat());
        while(Serial1.available() == 0){}
        if(Serial1.read() == 'W')
          latitude = -latitude;
      }
      num_sats = Serial1.parseInt();      //parse the number of satalites
      Serial1.find(',');
      while(Serial1.available() == 0){}
      if(Serial1.peek() != ','){
        HDOP = Serial1.parseFloat();      //parse HDOP
        altitude = Serial1.parseInt();      //parse height (in feet)
        VDOP = Serial1.parseFloat();      //parse HDOP
      }
      while(Serial1.available() == 0){}
      do{
        //if(Serial1.peek() == ',')
          Serial1.read();
          while(Serial1.available() == 0){}
      }while(Serial1.peek() == ',');
      Serial1.readBytesUntil('*', hex_voltage, 3);
      sscanf(hex_voltage, "%x", &Vbat);
      
      // for debuging
      #ifdef SERIAL_DEBUGING
      Serial.print("time: ");
      Serial.println(time);
      Serial.print("num_sats: ");
      Serial.println(num_sats);
      Serial.print("longditude: ");
      Serial.println(longditude, 4);
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
      
    }
      }
    }
