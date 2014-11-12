/*
 Arduino Library for Analog Devices ADXL362 - Micropower 3-axis accelerometer
 go to http://www.analog.com/ADXL362 for datasheet
 
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2012
 by Anne Mahaffey - hosted on http://annem.github.com/ADXL362
 Modified Mars 2014
 by pixelk
 Modified for Spark Core/Button October 2014
 by jenesaisdiq
 
 */ 

#include "application.h"

#ifndef SparkButton_h
#define SparkButton_h

class SparkButton {

 public:

  SparkButton();
  
  void
    begin(void),
    allLedsOff(void),
    allLedsOn(uint8_t r, uint8_t g, uint8_t b),
    ledOn(uint8_t i, uint8_t r, uint8_t g, uint8_t b),
    ledOff(uint8_t i),
    rainbow(uint8_t wait),
    smoothLedOn(float i, uint8_t r, uint8_t g, uint8_t b);
  uint8_t
    buttonOn(uint8_t i),
    allButtonsOn(void),
    allButtonsOff(void),
    lowestLed(void);
  int
    readX(void),
    readY(void),
    readZ(void),
    readX16(void),
    readY16(void),
    readZ16(void);
  //float
    //lowDirection(void);
    
 private:

};

//------------- Accelerometer --------------

/**
 * Turn debugging on or off
 */
#define ADXL362_DEBUG 0

/* ADXL Registers */

#define XL362_DEVID_AD      0x00
#define XL362_DEVID_MST     0x01
#define XL362_PARTID      0x02
#define XL362_REVID     0x03
#define XL362_XDATA     0x08
#define XL362_YDATA     0x09
#define XL362_ZDATA     0x0A
#define XL362_STATUS      0x0B
#define XL362_FIFO_ENTRIES_L    0x0C
#define XL362_FIFO_ENTRIES_H    0x0D
#define XL362_XDATA_L     0x0E
#define XL362_XDATA_H     0x0F
#define XL362_YDATA_L     0x10
#define XL362_YDATA_H     0x11
#define XL362_ZDATA_L     0x12
#define XL362_ZDATA_H     0x13
#define XL362_TEMP_L      0x14
#define XL362_TEMP_H      0x15
#define XL362_SOFT_RESET    0x1F
#define XL362_THRESH_ACT_L    0x20
#define XL362_THRESH_ACT_H    0x21
#define XL362_TIME_ACT        0x22
#define XL362_THRESH_INACT_L  0x23
#define XL362_THRESH_INACT_H  0x24
#define XL362_TIME_INACT_L    0x25
#define XL362_TIME_INACT_H    0x26
#define XL362_ACT_INACT_CTL   0x27
#define XL362_FIFO_CONTROL    0x28
#define XL362_FIFO_SAMPLES    0x29
#define XL362_INTMAP1     0x2A
#define XL362_INTMAP2     0x2B
#define XL362_FILTER_CTL    0x2C
#define XL362_POWER_CTL     0x2D
#define XL362_SELF_TEST     0x2E

/* Configuration values */

#define XL362_FILTER_FLAG_2G 0b00000000
#define XL362_FILTER_FLAG_4G 0b01000000
#define XL362_FILTER_FLAG_8G 0b10000000

#define XL362_FILTER_FLAG_HBW 0b10000
#define XL362_FILTER_FLAG_FBW 0b00000

#define XL362_FILTER_FLAG_ODR12  0b000
#define XL362_FILTER_FLAG_ODR25  0b001
#define XL362_FILTER_FLAG_ODR50  0b010
#define XL362_FILTER_FLAG_ODR100 0b011
#define XL362_FILTER_FLAG_ODR200 0b100
#define XL362_FILTER_FLAG_ODR400 0b111

#define XL362_POWER_FLAG_NOISE_NORMAL   0b000000
#define XL362_POWER_FLAG_NOISE_LOW      0b010000
#define XL362_POWER_FLAG_NOISE_ULTRALOW 0b100000

#define XL362_POWER_FLAG_MEASURE_STANDBY 0b00
#define XL362_POWER_FLAG_MEASURE_RUNING  0b10

class ADXL362
{
public:

  ADXL362();
  
  //
  // Basic Device control and readback functions
  //
  void begin();     
  void beginMeasure(); 
  int readX16();
  int readY16();
  int readZ16();
  int readX();
  int readY();
  int readZ();
  void readXYZTData(short &XData, short &YData, short &ZData, float &Temperature);
  void readXYZmg(int &X, int &Y, int &Z);
  void XYZmgtoRPT(int X, int Y, int Z, float &Rho, float &Phi, float &Theta);
  int16_t readTemp();
  
  // need to add the following functions
  // void mapINT1(
  // void mapINT2
  // void autoSleep
  // void activityInterruptControl
  //    -Activity, Inactivity, Both
  //    - Referenced, Absolute
  //    - Free Fall, Linked Mode, Loop Mode
  
  
  void checkAllControlRegs();
  
  void setRange(uint8_t Range);
  void setBandwidth(uint8_t BandWidth);
  void setOutputDatarate(uint8_t ODR);
  void setNoiseLevel(uint8_t NoiseLevel);
  
  //  Low-level SPI control, to simplify overall coding
  uint8_t SPIreadOneRegister(uint8_t regAddress);
  void SPIwriteOneRegister(uint8_t regAddress, uint8_t regValue);
  int  SPIreadTwoRegisters(uint8_t regAddress);
  void SPIwriteTwoRegisters(uint8_t regAddress, int twoRegValue);
  
private:
  uint8_t mgperLSB; // default +-2g XL362_FILTER_FLAG_2G -> 1mg/LSB (ADXL362 Datasheet page 4)

};




//----------------- LED Handling ------------------------
#define PIXEL_COUNT 11
#define PIXEL_PIN A7
#define PIXEL_TYPE WS2812B

/*-------------------------------------------------------------------------
  Spark Core library to control WS2811/WS2812 based RGB
  LED devices such as Adafruit NeoPixel strips.
  Currently handles 800 KHz and 400kHz bitstream on Spark Core, 
  WS2812, WS2812B and WS2811.

  Also supports Radio Shack Tri-Color Strip with TM1803 controller
  400kHz bitstream.

  Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.
  Modified to work with Spark Core by Technobly.
  Modified to work with Spark Button by jenesaisdiq.
  Contributions by PJRC and other members of the open source community.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!
  --------------------------------------------------------------------*/
  
/* ======================= Adafruit_NeoPixel.h ======================= */  
/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/
  
// 'type' flags for LED pixels (third parameter to constructor):
#define WS2812B  0x02 // 800 KHz datastream (NeoPixel)
  
class Adafruit_NeoPixel {

 public:

  // Constructor: number of LEDs, pin number, LED type
  Adafruit_NeoPixel(uint16_t n, uint8_t p=2, uint8_t t=WS2812B);
  ~Adafruit_NeoPixel();

  void
    begin(void),
    show(void),
    setPin(uint8_t p),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    setPixelColor(uint16_t n, uint32_t c),
    setBrightness(uint8_t);
  uint8_t
   *getPixels() const;
  uint16_t
    numPixels(void) const;
  static uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b);
  uint32_t
    getPixelColor(uint16_t n) const;

 private:

  const uint16_t
    numLEDs,       // Number of RGB LEDs in strip
    numBytes;      // Size of 'pixels' buffer below
  const uint8_t
    type;          // Pixel type flag (400 vs 800 KHz)
  uint8_t
    pin,           // Output pin number
    brightness,
   *pixels;        // Holds LED color values (3 bytes each)
  uint32_t
    endTime;       // Latch timing reference
};

#endif
