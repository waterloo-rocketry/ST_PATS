#ifndef DISPLAY_H
#define DISPLAY_H
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 5
#define TFT_CS 7
#define BRIGHTNESS 255
#define NAV_BALL_CENTER_X_CORD 120
#define NAV_BALL_CENTER_Y_CORD 120
#define NAV_BALL_RADIUS 100



class Display {
  private:
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    
  public:
    int heading;
    int previous_heading;
    float battery_voltage;
    Display(){
      heading = 0;
      previous_heading = 0;
      battery_voltage = 0;
    }
    void begin();
    void set_background();
    void Update();
    void draw_arrow(int heading);
};

void rotate(float & x_cord, float & y_cord, float theta);
#endif
