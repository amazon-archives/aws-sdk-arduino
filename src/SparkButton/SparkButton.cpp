#include "SparkButton.h"
#include "math.h"

//----------------- Button ----------------//

Adafruit_NeoPixel ring = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
ADXL362 accelerometer;

SparkButton::SparkButton(){
    
}

void SparkButton::begin(){
    ring.begin();
    ring.show();
    
    accelerometer.begin();                   // Setup SPI protocol, issue device soft reset
    accelerometer.beginMeasure();            // Switch ADXL362 to measure mode  
    accelerometer.checkAllControlRegs();     // Burst Read all Control Registers, to check for proper setup
    
    pinMode(D1, INPUT_PULLUP);
    pinMode(D2, INPUT_PULLUP);
    pinMode(D3, INPUT_PULLUP);
    pinMode(D4, INPUT_PULLUP);
}

void SparkButton::ledOn(uint8_t i, uint8_t r, uint8_t g, uint8_t b){
    //i-1 shifts the location from human readable to the right index for the LEDs
    if(i == 12){
        ring.setPixelColor(0, ring.Color(r,g,b));
        ring.setPixelColor(10, ring.Color(r,g,b));
    }
    else{
        ring.setPixelColor(i-1, ring.Color(r,g,b));
    }
    ring.show();
}

void SparkButton::smoothLedOn(float i, uint8_t r, uint8_t g, uint8_t b){
    //uint8_t intI = lrintf(i);
    //Serial.print("intI: ");
    //Serial.println(intI);
    
    //float differ = i-(float)intI + 0.5;
    //Serial.print("differ: ");
    //Serial.println(differ);
    
    float tempI;
    float differ = modff(i, &tempI);
    uint8_t intI = (uint8_t)tempI;
    
    
    // checks to see if it's reeeeally close to being an integer
    //if(abs(differ) < 0.01){
      // intI-1 shifts the location from human readable to the right index for the LEDs
    //  ring.setPixelColor(intI-1, ring.Color(r,g,b));
    //  Serial.println("tripped int check");
    //}
    //else {
      // diff > 0 means that it's closer to the lower one
      float differ1 = 1.0-differ;
      if(differ > 0.5){
        ring.setPixelColor(intI-1, ring.Color((int)(differ1*r),(int)(differ1*g),(int)(differ1*b)));
        ring.setPixelColor(intI, ring.Color((int)(differ*r),(int)(differ*g),(int)(differ*b))); 
      }
      else {
        ring.setPixelColor(intI-1, ring.Color((int)(differ1*r),(int)(differ1*g),(int)(differ1*b)));
        ring.setPixelColor(intI, ring.Color((int)(differ*r),(int)(differ*g),(int)(differ*b)));
      }
    //}
    ring.show();
}

void SparkButton::ledOff(uint8_t i){
    //i-1 shifts the location from human readable to the right index for the LEDs
    if(i == 12){
        ring.setPixelColor(0, ring.Color(0,0,0));
        ring.setPixelColor(10, ring.Color(0,0,0));
    }
    else{
        ring.setPixelColor(i-1, ring.Color(0,0,0));
    }
    ring.show();
}

void SparkButton::allLedsOff(){
    for(int i = 0; i<PIXEL_COUNT; i++){
            ring.setPixelColor(i, ring.Color(0, 0, 0));
    }
    ring.show();
}

void SparkButton::allLedsOn(uint8_t r, uint8_t g, uint8_t b){
    for(int i = 0; i<PIXEL_COUNT; i++){
            ring.setPixelColor(i, ring.Color(r, g, b));
    }
    ring.show();
}

uint8_t SparkButton::buttonOn(uint8_t i){
    return !digitalRead(i);
}

uint8_t SparkButton::allButtonsOn(){
    if(!digitalRead(D1) && !digitalRead(D2) && !digitalRead(D3) && !digitalRead(D4)) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t SparkButton::allButtonsOff(){
    if(digitalRead(D1) && digitalRead(D2) && digitalRead(D3) && digitalRead(D4)) {
        return 1;
    }
    else {
        return 0;
    }
}

void SparkButton::rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< 12; i++) {
      uint8_t WheelPos = ((i * 256 / ring.numPixels()) + j) & 255;
      if(WheelPos < 85) {
         ring.setPixelColor(i,ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0));
      } else if(WheelPos < 170) {
         WheelPos -= 85;
         ring.setPixelColor(i,ring.Color(255 - WheelPos * 3, 0, WheelPos * 3));
      } else {
         WheelPos -= 170;
         ring.setPixelColor(i,ring.Color(0, WheelPos * 3, 255 - WheelPos * 3));
      }
    }
    ring.show();
    delay(wait);
  }
}

int SparkButton::readX(){
    return accelerometer.readX();
}

int SparkButton::readY(){
    return accelerometer.readY();
}

int SparkButton::readZ(){
    return accelerometer.readZ();
}

int SparkButton::readX16(){
    return accelerometer.readX16();
}

int SparkButton::readY16(){
    return accelerometer.readY16();
}

int SparkButton::readZ16(){
    return accelerometer.readZ16();
}

//Thanks christophevg!
uint8_t SparkButton::lowestLed(){
    float rads = atan2(accelerometer.readY16(),accelerometer.readX16());
    uint8_t ledPos = (uint8_t)(12 - (rads/(M_PI/6) - 3)) % 12;
    return ledPos;
}

//float SparkButton::lowDirection(){
//  float rads = atan2(this.readY16(),this.readX16());
//  float ledPos = abs(rads/(M_PI/12)) + 1;
//  if(ledPos == 0 || ledPos > 12){
//    ledPos = 12;
//  }
//  return ledPos;
//}


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


const int slaveSelectPin = A2;

ADXL362::ADXL362() {}

//
//  begin()
//  Initial SPI setup, soft reset of device
//
void ADXL362::begin() {
  pinMode(slaveSelectPin, OUTPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0); //CPHA = CPOL = 0    MODE = 0
  delay(500);
    
  // soft reset
  mgperLSB = 1;
  SPIwriteOneRegister(XL362_SOFT_RESET, 0x52);  // Write to SOFT RESET, "R"
  delay(10);
#ifdef ADXL362_DEBUG 
  Serial.println("Soft Reset\n");
#endif
 }

 
//
//  beginMeasure()
//  turn on Measurement mode - required after reset
// 
void ADXL362::beginMeasure() {
  uint8_t temp = SPIreadOneRegister(XL362_POWER_CTL); // read Reg 2D before modifying for measure mode
#ifdef ADXL362_DEBUG
  Serial.print(  "Setting Measeurement Mode - Reg XL362_POWER_CTL before = "); 
  Serial.print(temp);
#endif

  // turn on measurement mode
  temp = (temp & 0b11111100) | XL362_POWER_FLAG_MEASURE_RUNING;     // turn on measurement bit in Reg XL362_POWER_CTL
  SPIwriteOneRegister(XL362_POWER_CTL, temp); // Write to XL362_POWER_CTL, Measurement Mode
  delay(10);  
  
#ifdef ADXL362_DEBUG
  temp = SPIreadOneRegister(XL362_POWER_CTL);
  Serial.print(  ", Reg XL362_POWER_CTL after = "); 
  Serial.println(temp); 
#endif
}



//reading off the 8-bit register as documented in the ADXL362 spec
//IMPORTANT to make it a signed 8-bit int so that the data is interpreted correctly
int ADXL362::readX(){
  int8_t XDATA = SPIreadOneRegister(0x08);
//#ifdef ADXL362_DEBUG
//  Serial.print(  "XDATA = "); 
//  Serial.println(XDATA);
//#endif
  return (int)XDATA;
}

int ADXL362::readY(){
  int8_t YDATA = SPIreadOneRegister(0x09);
//#ifdef ADXL362_DEBUG
//  Serial.print(  "\tYDATA = "); 
//  Serial.println(YDATA);
//#endif
  return (int)YDATA;
}

int ADXL362::readZ(){
  int8_t ZDATA = SPIreadOneRegister(0x0A);
//#ifdef ADXL362_DEBUG
//  Serial.print(  "\tZDATA = "); 
//  Serial.println(ZDATA);
//#endif
  return (int)ZDATA;
}

//
//  readXData(), readYData(), readZData(), readTemp()
//  Read X, Y, Z, and Temp registers
//
int ADXL362::readX16(){
  int16_t XDATA = SPIreadTwoRegisters(XL362_XDATA_L);
#ifdef ADXL362_DEBUG
  Serial.print(  "XDATA = "); 
  Serial.println(XDATA);
#endif
  return XDATA;
}

int ADXL362::readY16(){
  int16_t YDATA = SPIreadTwoRegisters(XL362_YDATA_L);
#ifdef ADXL362_DEBUG
  Serial.print(  "\tYDATA = "); 
  Serial.println(YDATA);
#endif
  return YDATA;
}

int ADXL362::readZ16(){
  int16_t ZDATA = SPIreadTwoRegisters(XL362_ZDATA_L);
#ifdef ADXL362_DEBUG
  Serial.print(  "\tZDATA = "); 
  Serial.println(ZDATA);
#endif
  return ZDATA;
}

//Temperature only has a 16-bit version, so read two 8-bit regs
//worth knowing that this is an INTERNAL temperature measurement, so doesn't reflect the environment accurately
int16_t ADXL362::readTemp(){
  int16_t TEMP = SPIreadTwoRegisters(XL362_TEMP_L);
//#ifdef ADXL362_DEBUG
//  Serial.print("\tTEMP = "); 
//  Serial.println(TEMP);
//#endif
  return TEMP;
}

void ADXL362::readXYZTData(short &XData, short &YData, short &ZData, float &Temperature){
  
  // burst SPI read
  // A burst read of all three axis is required to guarantee all measurements correspond to same sample time
  digitalWrite(slaveSelectPin, LOW);
  
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(XL362_XDATA_L);  // Start at XData Reg
  XData = SPI.transfer(0x00);
  XData = XData + ((short)SPI.transfer(0x00) << 8);
  YData = SPI.transfer(0x00);
  YData = YData + ((short)SPI.transfer(0x00) << 8);
  ZData = SPI.transfer(0x00);
  ZData = ZData + ((short)SPI.transfer(0x00) << 8);
  short RawTemperature = SPI.transfer(0x00);
  RawTemperature = RawTemperature + ((short)SPI.transfer(0x00) << 8);
  Temperature = (float)RawTemperature * 0.065;
  digitalWrite(slaveSelectPin, HIGH);
  
#ifdef ADXL362_DEBUG
  Serial.print(  "XDATA = "); Serial.print(XData); 
  Serial.print(  "\tYDATA = "); Serial.print(YData); 
  Serial.print(  "\tZDATA = "); Serial.print(ZData); 
  Serial.println(  "\tTemperature = "); Serial.println(Temperature);
#endif
}

void ADXL362::readXYZmg(int &X, int &Y, int &Z){
  // burst SPI read
  // A burst read of all three axis is required to guarantee all measurements correspond to same sample time
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(XL362_XDATA_L);  // Start at XData Reg
  short XData = SPI.transfer(0x00);
  XData = XData + ((short)SPI.transfer(0x00) << 8);
  short YData = SPI.transfer(0x00);
  YData = YData + ((short)SPI.transfer(0x00) << 8);
  short ZData = SPI.transfer(0x00);
  ZData = ZData + ((short)SPI.transfer(0x00) << 8);
  digitalWrite(slaveSelectPin, HIGH);
  
  X = (int)XData * mgperLSB;
  Y = (int)YData * mgperLSB;
  Z = (int)ZData * mgperLSB;
  
#ifdef ADXL362_DEBUG
  Serial.print(  "x = "); Serial.print(X); 
  Serial.print(  "\ty = "); Serial.print(Y); 
  Serial.println(  "\tz = "); Serial.print(Z); 
#endif
}

void ADXL362::XYZmgtoRPT(int X, int Y, int Z, float &Rho, float &Phi, float &Theta){
  Rho = atan2(float(X), sqrt(pow(float(Y),2)+pow(float(Z),2)));
  Rho *= 180/M_PI; 

  Phi = atan2(float(Y), sqrt(pow(float(X),2)+pow(float(Z),2)));
  Phi *= 180/M_PI; 

  Theta = atan2(sqrt(pow(float(X),2)+pow(float(Y),2)),float(Z));
  Theta *= 180/M_PI;  
}

void ADXL362::checkAllControlRegs(){
  //uint8_t filterCntlReg = SPIreadOneRegister(0x2C);
  //uint8_t ODR = filterCntlReg & 0x07;  Serial.print("ODR = ");  Serial.println(ODR, HEX);
  //uint8_t ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);      Serial.print("ACT_INACT_CTL_Reg = "); Serial.println(ACT_INACT_CTL_Reg, HEX);
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(0x20);  // Start burst read at Reg 20
  Serial.println("Start Burst Read of all Control Regs - Library version 6-24-2012:");
  Serial.print("Reg XL362_THRESH_ACT_L   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_THRESH_ACT_H   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_TIME_ACT       = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_THRESH_INACT_L = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_THRESH_INACT_H = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_TIME_INACT_L   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_TIME_INACT_H   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_ACT_INACT_CTL  = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_FIFO_CONTROL   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_FIFO_SAMPLES   = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_INTMAP1        = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_INTMAP2        = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_FILTER_CTL     = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_POWER_CTL      = B");   Serial.println(SPI.transfer(0x00), BIN);
  Serial.print("Reg XL362_SELF_TEST      = B");   Serial.println(SPI.transfer(0x00), BIN);
  
  digitalWrite(slaveSelectPin, HIGH);
}

void ADXL362::setRange(uint8_t Range){
  // Modify range (+-2g +-4g +-8g - ADXL362 Datasheep Page 33
  // Choose RangeFlag between XL362_FILTER_FLAG_2G (default), XL362_FILTER_FLAG_4G, XL362_FILTER_FLAG_8G
  uint8_t temp = SPIreadOneRegister(XL362_FILTER_CTL);  // read Reg XL362_FILTER_CTL before modifying
#ifdef ADXL362_DEBUG
  Serial.print(  "Setting Measurement Range - Reg XL362_FILTER_CTL before = "); 
  Serial.print(temp);
#endif

  switch ( Range ) { // Range affects converting LSB to mg
  case XL362_FILTER_FLAG_2G:
    mgperLSB = 1;
    break;
  case XL362_FILTER_FLAG_4G:
    mgperLSB = 2;
    break;
  case XL362_FILTER_FLAG_8G:
    mgperLSB = 4;
    break;
  default:
    // YOU SHOULDN'T BE HERE !
    mgperLSB = 1;
    break;
  }

  temp = temp & 0b00111111 | Range;
  SPIwriteOneRegister(XL362_FILTER_CTL, temp); // Write to XL362_FILTER_CTL
  delay(10);  
  
#ifdef ADXL362_DEBUG
  temp = SPIreadOneRegister(XL362_FILTER_CTL);
  Serial.print(  ", Reg after = "); 
  Serial.println(temp); 
#endif
}

void ADXL362::setBandwidth(uint8_t BandWidth){
  // modify Bandwidth - ADXL362 Datasheep Page 33
  // Choose Bandwidth between XL362_FILTER_FLAG_HBW (default), XL362_FILTER_FLAG_FBW
  uint8_t temp = SPIreadOneRegister(XL362_FILTER_CTL);  // read Reg XL362_FILTER_CTL before modifying
#ifdef ADXL362_DEBUG
  Serial.print(  "Setting BandWidth - Reg XL362_FILTER_CTL before = "); 
  Serial.print(temp);
#endif

  temp = temp & 0b11101111 | BandWidth;
  SPIwriteOneRegister(XL362_FILTER_CTL, temp); // Write to XL362_FILTER_CTL
  delay(10);  
  
#ifdef ADXL362_DEBUG
  temp = SPIreadOneRegister(XL362_FILTER_CTL);
  Serial.print(  ", Reg after = "); 
  Serial.println(temp); 
#endif
}

void ADXL362::setOutputDatarate(uint8_t ODR){
  // modify Output Data Rate - ADXL362 Datasheep Page 33
  // Choose ODR between  XL362_FILTER_FLAG_ODR12, XL362_FILTER_FLAG_ODR25, XL362_FILTER_FLAG_ODR50, XL362_FILTER_FLAG_ODR100 (default), XL362_FILTER_FLAG_ODR200 , XL362_FILTER_FLAG_ODR400
  uint8_t temp = SPIreadOneRegister(XL362_FILTER_CTL);  // read Reg XL362_FILTER_CTL before modifying
#ifdef ADXL362_DEBUG
  Serial.print(  "Setting Output Data Rate - Reg XL362_FILTER_CTL before = "); 
  Serial.print(temp);
#endif

  temp = temp & 0b11111000 | ODR;
  SPIwriteOneRegister(XL362_FILTER_CTL, temp); // Write to XL362_FILTER_CTL
  delay(10);  
  
#ifdef ADXL362_DEBUG
  temp = SPIreadOneRegister(XL362_FILTER_CTL);
  Serial.print(  ", Reg after = "); 
  Serial.println(temp); 
#endif
}

void ADXL362::setNoiseLevel(uint8_t NoiseLevel){
  // modify Noise Level - ADXL362 Datasheep Page 34
  // Choose NoiseLevel between XL362_POWER_FLAG_NOISE_NORMAL (default), XL362_POWER_FLAG_NOISE_LOW, XL362_POWER_FLAG_NOISE_ULTRALOW
  uint8_t temp = SPIreadOneRegister(XL362_POWER_CTL); // read Reg XL362_FILTER_CTL before modifying
#ifdef ADXL362_DEBUG
  Serial.print(  "Setting Output Data Rate - Reg XL362_POWER_CTL before = "); 
  Serial.print(temp);
#endif

  temp = temp & 0b11001111  | NoiseLevel;
  SPIwriteOneRegister(XL362_POWER_CTL, temp); // Write to XL362_FILTER_CTL
  delay(10);  
  
#ifdef ADXL362_DEBUG
  temp = SPIreadOneRegister(XL362_POWER_CTL);
  Serial.print(  ", Reg after = "); 
  Serial.println(temp); 
#endif
}

// Basic SPI routines to simplify code
// read and write one register

uint8_t ADXL362::SPIreadOneRegister(uint8_t regAddress){
  uint8_t regValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);
  regValue = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  return regValue;
}

void ADXL362::SPIwriteOneRegister(uint8_t regAddress, uint8_t regValue){
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);
  SPI.transfer(regValue);
  digitalWrite(slaveSelectPin, HIGH);
}

int ADXL362::SPIreadTwoRegisters(uint8_t regAddress){
  int twoRegValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);  
  twoRegValue = SPI.transfer(0x00);
  twoRegValue = twoRegValue + (SPI.transfer(0x00) << 8);
  digitalWrite(slaveSelectPin, HIGH);

  return twoRegValue;
}  

void ADXL362::SPIwriteTwoRegisters(uint8_t regAddress, int twoRegValue){
  
  uint8_t twoRegValueH = twoRegValue >> 8;
  uint8_t twoRegValueL = twoRegValue;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);  
  SPI.transfer(twoRegValueL);
  SPI.transfer(twoRegValueH);
  digitalWrite(slaveSelectPin, HIGH);
}


//----------------- LED Handling ------------------------//

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

/* ======================= Adafruit_NeoPixel.cpp ======================= */
/*-------------------------------------------------------------------------
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
  -------------------------------------------------------------------------*/

Adafruit_NeoPixel::Adafruit_NeoPixel(uint16_t n, uint8_t p, uint8_t t) : \
  numLEDs(n), numBytes(n*3), type(t), pin(p), pixels(NULL)
{
  if((pixels = (uint8_t *)malloc(numBytes))) {
    memset(pixels, 0, numBytes);
  }
}

Adafruit_NeoPixel::~Adafruit_NeoPixel() {
  if(pixels) free(pixels);
  pinMode(pin, INPUT);
}

void Adafruit_NeoPixel::begin(void) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  endTime = micros();
}

void Adafruit_NeoPixel::show(void) {
  if(!pixels) return;

  // Data latch = 24 or 50 microsecond pause in the output stream.  Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed.  This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  while((micros() - endTime) < 50L);
  
  // endTime is a private member (rather than global var) so that multiple
  // instances on different pins can be quickly issued in succession (each
  // instance doesn't delay the next).

  __disable_irq(); // Need 100% focus on instruction timing

  volatile uint32_t 
    c,    // 24-bit pixel color
    mask; // 8-bit mask
  volatile uint16_t i = numBytes; // Output loop counter
  volatile uint8_t
    j,              // 8-bit inner loop counter
   *ptr = pixels,   // Pointer to next byte
    g,              // Current green byte value
    r,              // Current red byte value
    b;              // Current blue byte value
  
  if(type == WS2812B) { // same as WS2812, 800 KHz bitstream
    while(i) { // While bytes left... (3 bytes = 1 pixel)
      mask = 0x800000; // reset the mask
      i = i-3;      // decrement bytes remaining
      g = *ptr++;   // Next green byte value
      r = *ptr++;   // Next red byte value
      b = *ptr++;   // Next blue byte value
      c = ((uint32_t)g << 16) | ((uint32_t)r <<  8) | b; // Pack the next 3 bytes to keep timing tight
      j = 0;        // reset the 24-bit counter
      do {
        PIN_MAP[pin].gpio_peripheral->BSRR = PIN_MAP[pin].gpio_pin; // HIGH
        if (c & mask) { // if masked bit is high
          // WS2812 spec             700ns HIGH
          // Adafruit on Arduino    (meas. 812ns)
          // This lib on Spark Core (meas. 804ns)
          asm volatile(
            "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t"
            ::: "r0", "cc", "memory");
          // WS2812 spec             600ns LOW
          // Adafruit on Arduino    (meas. 436ns)
          // This lib on Spark Core (meas. 472ns)
          PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin; // LOW
        }
        else { // else masked bit is low
          // WS2812 spec             350ns HIGH
          // Adafruit on Arduino    (meas. 312ns)
          // This lib on Spark Core (meas. 316ns)
          asm volatile(
            "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t"
            ::: "r0", "cc", "memory");
          // WS2812 spec             800ns LOW
          // Adafruit on Arduino    (meas. 938ns)
          // This lib on Spark Core (meas. 932ns)
          PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin; // LOW
          asm volatile(
            "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
            "nop" "\n\t" "nop" "\n\t"
            ::: "r0", "cc", "memory");
        }
        mask >>= 1;
      } while ( ++j < 24 ); // ... pixel done
    } // end while(i) ... no more pixels
  }
  
  __enable_irq();
  endTime = micros(); // Save EOD time for latch on next call
}

// Set the output pin number
void Adafruit_NeoPixel::setPin(uint8_t p) {
  pinMode(pin, INPUT);
  pin = p;
  pinMode(p, OUTPUT);
  digitalWrite(p, LOW);
}

// Set pixel color from separate R,G,B components:
void Adafruit_NeoPixel::setPixelColor(
 uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numLEDs) {
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
    //if(type == WS2812B) { // WS2812/WS2812B is GRB order.
      *p++ = g;
      *p++ = r;
    //} 
    *p = b;
  }
}

// Set pixel color from 'packed' 32-bit RGB color:
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint32_t c) {
  if(n < numLEDs) {
    uint8_t
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
    //if(type == WS2812B) { // WS2812/WS2812B is GRB order.
      *p++ = g;
      *p++ = r;
    //} 
    *p = b;
  }
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t Adafruit_NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Query color from previously-set pixel (returns packed 32-bit RGB value)
uint32_t Adafruit_NeoPixel::getPixelColor(uint16_t n) const {

  if(n < numLEDs) {
    uint16_t ofs = n * 3;
    return (uint32_t)(pixels[ofs + 2]) |
      ((uint32_t)(pixels[ofs    ]) <<  8) |
      ((uint32_t)(pixels[ofs + 1]) << 16);
  }

  return 0; // Pixel # is out of bounds
}

uint8_t *Adafruit_NeoPixel::getPixels(void) const {
  return pixels;
}

uint16_t Adafruit_NeoPixel::numPixels(void) const {
  return numLEDs;
}

// Adjust output brightness; 0=darkest (off), 255=brightest.  This does
// NOT immediately affect what's currently displayed on the LEDs.  The
// next call to show() will refresh the LEDs at this level.  However,
// this process is potentially "lossy," especially when increasing
// brightness.  The tight timing in the WS2811/WS2812 code means there
// aren't enough free cycles to perform this scaling on the fly as data
// is issued.  So we make a pass through the existing color data in RAM
// and scale it (subsequent graphics commands also work at this
// brightness level).  If there's a significant step up in brightness,
// the limited number of steps (quantization) in the old data will be
// quite visible in the re-scaled version.  For a non-destructive
// change, you'll need to re-render the full strip data.  C'est la vie.
void Adafruit_NeoPixel::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  uint8_t newBrightness = b + 1;
  if(newBrightness != brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t  c,
            *ptr           = pixels,
             oldBrightness = brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<numBytes; i++) {
      c      = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    brightness = newBrightness;
  }
}
