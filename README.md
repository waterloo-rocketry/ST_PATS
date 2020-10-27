# ST_PATS (System That Points At The Shark)
ST_PATS was originally desinged in 2019 to provide a compact handheld system for tracking the rocket. It recieves the rocket's GPS location from two independent radios, and compares it to an internal GPS. This allows it to calculate the distance and direction to the rocket in real time. This means that with the use of a compass the rocket can easily be tracked to within a meter (this is super overkill lol, most people have eyes).


## Dependencies
There are a few things that the ST_PATS code needs in order to build. the first is the libraries:
- Adafruit GPS library
- Adafruit Unified Sensor library
- Adafruit LSM303DLHC library
- Adafruit GFX library
- Adafruit ILI9341 library

A tutorial of how to download libraries can be found here: https://www.arduino.cc/en/Guide/Libraries

You also need to install suport for the STM32 Nucleo 144, which uses the STM32L496ZG processor.:
- STM32 Cores

A tutorial of how to insatll cores can be found here: https://www.arduino.cc/en/Guide/Cores

## Parts list
Here is a list of major parts used in the current ST_PATS:
- Adafruit Ultimate GPS Breakout: https://www.adafruit.com/product/746
- Adafruit LSM303 Breakout: https://www.adafruit.com/product/1120 (NOTE: Due to EMI I could never get the internal compass working right, so we don't really do anything with this)
- 2x whatever radios we need this year (2019 was 2 differnet modles of xbees)
- Adafruit ILI9341 2.8" TFT: https://www.adafruit.com/product/1770
- 4x Adafruit Rugged Metal Push Buttons: https://www.adafruit.com/product/481 (Red, Blue, and Green momentary, White latching)
- Adafruit Lithum Ion Backpack for Itsy Bitsy: https://www.adafruit.com/product/2124
- Adafruit 4400mAh Lithium Ion Battery Pack: https://www.adafruit.com/product/354