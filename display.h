#ifndef DISPLAY_H
#define DISPLAY_H
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
#define BRIGHTNESS 255
#define NAV_BALL_CENTER_X_CORD 120
#define NAV_BALL_CENTER_Y_CORD 120
#define NAV_BALL_RADIUS 100


class Display {
  private:
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    
  public:
    Display();
    void begin();
    void set_background();
    void write_data(int system_cal, int gyro_cal, int accel_cal, int mag_cal, float latitude, float longditude);
    void draw_arrow(int heading);
};

void rotate(float & x_cord, float & y_cord, float theta);
#endif
