/*
PacketSize:
|   #0  |#1|#2|#3|#4|#5|#6|   #7  |   #8   |   #9  | #10  |
|Command|LR|LG|LB|HR|HG|HB|LowTemp|HighTemp|HIGHLed|LowLED|
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>    //#include <FastLED.h> for fastled

#include "sysSerial.h"

#define BUS PB0
#define LED PB4

//Dallas OneWireBus PB0; //ATtiny85 (BUS)
OneWire OW(BUS);
DallasTemperature DS(&OW);
Adafruit_NeoPixel px;//(512,LED,NEO_GRB + NEO_KHZ800);

void setup() {
  SerialSetup();
  LoadEEPROM();
  px = Adafruit_NeoPixel(_c.DC,LED,NEO_GRB + NEO_KHZ800);
  px.begin();
  for(uint16_t n=0; n < _c.DC; n++)
  {
    px.setPixelColor(n,px.Color(_c.HR,_c.HG,_c.HB));
    px.show();
    delay(10);
  }
  if(DS.getDS18Count() != 0)
    DS.setResolution(9);
}
void loop() {
  GetSerial();
  if(DS.getDS18Count() != 0) {
    DS.requestTemperatures();
    double temp = DS.getTempCByIndex(0);
  }
  delay(1000);
}
