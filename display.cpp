#include "display.h"


void Display::begin(){
  tft.begin();
}
void Display::set_background(){
    // draw outside circle
  tft.fillScreen(ILI9341_BLACK);
  tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, NAV_BALL_RADIUS, ILI9341_WHITE);
  tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 1), ILI9341_WHITE);
  tft.drawCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, (NAV_BALL_RADIUS + 2), ILI9341_WHITE);
}
void Display::Update(){
  
  
}
void Display::draw_arrow(int heading){
  tft.setCursor(0, 220);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Heading:");
  tft.println(heading);
    
  if(previous_heading != heading){
    tft.fillRect(0, 235, 40, 20, ILI9341_RED);
    previous_heading = heading;
    tft.fillCircle(NAV_BALL_CENTER_X_CORD, NAV_BALL_CENTER_Y_CORD, NAV_BALL_RADIUS - 1, ILI9341_BLACK);
    float x_cord_0 = 0, x_cord_1 = 0;
    float y_cord_0 = -NAV_BALL_RADIUS, y_cord_1 = NAV_BALL_RADIUS-10;
    /*rotate(x_cord_0, y_cord_0, previous_heading*PI/-180);
    rotate(x_cord_1, y_cord_1, previous_heading*PI/-180);
    tft.drawLine(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), ILI9341_BLACK);
    x_cord_0 = -NAV_BALL_RADIUS, x_cord_1 = NAV_BALL_RADIUS;
    y_cord_0 = 0, y_cord_1 = 0;
    */
    rotate(x_cord_0, y_cord_0, heading*PI/-180);
    rotate(x_cord_1, y_cord_1, heading*PI/-180);
    tft.drawLine(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), ILI9341_GREEN);
    tft.fillCircle(round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), 10, ILI9341_GREEN);
  }
}

void rotate(float & x_cord, float & y_cord, float theta){
  float temp_x_cord = x_cord;
  x_cord = (temp_x_cord*cos(theta)) - (y_cord*sin(theta));
  y_cord = (temp_x_cord*sin(theta)) + (y_cord*cos(theta));
}
