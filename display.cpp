#include "display.h"

Display::Display(){
  
}

void Display::power_up(){
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
}

void Display::power_down(){
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
}

void Display::begin(){
  tft.begin();
}

void Display::set_background(){
    // draw outside circle
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  //
  //tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 1), ILI9341_WHITE);
  //tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 2), ILI9341_WHITE);
}

void Display::write_GPS(String GPS_name, double time, double latitude, double longditude){
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(2);
  tft.println(GPS_name);
  tft.setTextColor(ILI9341_RED);
  tft.println("Time:");
  tft.println(time);
  tft.println("Lat:");
  tft.println(latitude, 5);
  tft.println("Long:");
  tft.println(longditude, 5);
}
void Display::write_local_data(double time, int num_sats){
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(2);
  tft.print("Local: ");
  tft.setTextColor(ILI9341_RED);
  tft.print(time);
  tft.print(" Sats:");
  tft.print(num_sats);
}

void Display::draw_arrow(int heading){
    //tft.fillCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, NAV_BALL_RADIUS, ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(4);
    tft.setCursor(NAV_BALL_CENTER_X_CORD - 20, NAV_BALL_CENTER_Y_CORD - 10);
    tft.println(heading);
    tft.setCursor(0, 1); // put the cursor back so we can write cordinates
    
    double x_cord_0 = 0, x_cord_1 = 0;
    double y_cord_0 = -NAV_BALL_RADIUS, y_cord_1 = NAV_BALL_RADIUS-10;
    
    rotate(x_cord_0, y_cord_0, heading*PI/-180);
    rotate(x_cord_1, y_cord_1, heading*PI/-180);
    
    tft.drawLine(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), ILI9341_GREEN);
    tft.fillCircle(round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), 10, ILI9341_GREEN);
}

void rotate(double & x_cord, double & y_cord, double theta){
  double temp_x_cord = x_cord;
  x_cord = (temp_x_cord*cos(theta)) - (y_cord*sin(theta));
  y_cord = (temp_x_cord*sin(theta)) + (y_cord*cos(theta));
}

void Display::reset(){
  tft.fillScreen(ILI9341_BLACK);
}
