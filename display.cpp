#include "display.h"

Display::Display(){
  
}
void Display::begin(){
  tft.begin();
}
void Display::set_background(){
    // draw outside circle
  tft.fillScreen(ILI9341_WHITE);
  //
  //tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 1), ILI9341_WHITE);
  //tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 2), ILI9341_WHITE);
}

void Display::write_data(int system_cal, int gyro_cal, int accel_cal, int mag_cal, float latitude, float longditude){
  tft.fillRect(0, 0, 240, 18, ILI9341_WHITE);
  tft.fillRect(0, 18, 60, 18, ILI9341_WHITE); // wipe the clibration text
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.print("Syst:");
  tft.print(system_cal);
  tft.print(" Gyro:");
  tft.print(gyro_cal);
  tft.print(" Acel:");
  tft.println(accel_cal);
  tft.print("Mag:");
  tft.print(mag_cal);
  tft.setCursor(0, 230);
  tft.print("Your Lat: ");
  tft.println(latitude);
  tft.print("Your Long: ");
  tft.println(longditude);
}

void Display::draw_arrow(int heading){
    tft.fillCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, NAV_BALL_RADIUS, ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(4);
    tft.setCursor(80, 100);
    tft.println(heading);
    
    float x_cord_0 = 0, x_cord_1 = 0;
    float y_cord_0 = -NAV_BALL_RADIUS, y_cord_1 = NAV_BALL_RADIUS-10;
    
    rotate(x_cord_0, y_cord_0, heading*PI/-180);
    rotate(x_cord_1, y_cord_1, heading*PI/-180);
    
    tft.drawLine(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), ILI9341_GREEN);
    tft.fillCircle(round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), 10, ILI9341_GREEN);
}

void rotate(float & x_cord, float & y_cord, float theta){
  float temp_x_cord = x_cord;
  x_cord = (temp_x_cord*cos(theta)) - (y_cord*sin(theta));
  y_cord = (temp_x_cord*sin(theta)) + (y_cord*cos(theta));
}
