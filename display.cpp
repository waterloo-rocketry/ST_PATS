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
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
}

void Display::write_GPS(String GPS_name, double time, double latitude, double longditude, bool state){
  if (state)
    tft.setTextColor(ILI9341_MAGENTA); 
  else
    tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(2);
  tft.print(GPS_name);
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.println(time);
  tft.print("Lat:");
  tft.println(latitude, 5);
  tft.print("Lon:");
  tft.println(longditude, 5);
}
void Display::write_local_data(int hour, int minutes, int seconds, int num_sats){
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(2);
  tft.println("Time: ");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.print(hour);
  tft.print(":");
  tft.print(minutes);
  tft.print(":");
  tft.println(seconds);
  tft.setTextColor(ILI9341_BLUE);
  tft.print("Sats:");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.println(num_sats);
  tft.print(" ");
}

void Display::draw_arrow(int heading){

/* UNTILL THE COMPASS IS WORKING THERE IS NO POINT DRAWING THE ARROW
    // erase the old triangle
    static int previous_heading = 0;
    double x_cord_0 = 0, x_cord_1 = -25, x_cord_2 = 25;
    double y_cord_0 = NAV_BALL_RADIUS + 8, y_cord_1 = NAV_BALL_RADIUS *-0.75 - 2, y_cord_2 = NAV_BALL_RADIUS *-0.75 - 2;
    
    rotate(x_cord_0, y_cord_0, previous_heading*PI/-180);
    rotate(x_cord_1, y_cord_1, previous_heading*PI/-180);
    rotate(x_cord_2, y_cord_2, previous_heading*PI/-180);
    
    tft.fillTriangle(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD),
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_2 + NAV_BALL_CENTER_X_CORD), round(y_cord_2 + NAV_BALL_CENTER_Y_CORD), ILI9341_WHITE);

    previous_heading = heading;

    // draw the new triangle
    
    x_cord_0 = 0, x_cord_1 = -20, x_cord_2 = 20;
    y_cord_0 = NAV_BALL_RADIUS, y_cord_1 = NAV_BALL_RADIUS *-0.75, y_cord_2 = NAV_BALL_RADIUS *-0.75;
    
    rotate(x_cord_0, y_cord_0, heading*PI/-180);
    rotate(x_cord_1, y_cord_1, heading*PI/-180);
    rotate(x_cord_2, y_cord_2, heading*PI/-180);
    
    tft.fillTriangle(round(x_cord_0 + NAV_BALL_CENTER_X_CORD), round(y_cord_0 + NAV_BALL_CENTER_Y_CORD),
    round(x_cord_1 + NAV_BALL_CENTER_X_CORD), round(y_cord_1 + NAV_BALL_CENTER_Y_CORD), 
    round(x_cord_2 + NAV_BALL_CENTER_X_CORD), round(y_cord_2 + NAV_BALL_CENTER_Y_CORD), ILI9341_BLACK);

*/
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(6); // 4 when drawing arrow
    tft.setCursor(160, 50); // (240, 1) when drawing arrow
    tft.print(heading);
    tft.print("  ");
    tft.setCursor(0, 1); // put the cursor back so we can write cordinates
}

void rotate(double & x_cord, double & y_cord, double theta){
  double temp_x_cord = x_cord;
  x_cord = (temp_x_cord*cos(theta)) - (y_cord*sin(theta));
  y_cord = (temp_x_cord*sin(theta)) + (y_cord*cos(theta));
}
