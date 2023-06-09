/*
PacketSize:
|   #0  |#1|#2|#3|#4|#5|#6|   #7  |   #8   |   #9  | #10  |
|Command|LR|LG|LB|HR|HG|HB|LowTemp|HighTemp|HIGHLed|LowLED|
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
#include "sysSerial.h"

#define BUS PB0
#define LED PB4

//Dallas OneWireBus PB2; //ATtiny85 (BUS)
OneWire OW(BUS);
DallasTemperature DS(&OW);
Adafruit_NeoPixel NP;

void setup() {
  SerialSetup();
  LoadEEPROM();
  if(_c.DC <= 512)
  {
    NP = Adafruit_NeoPixel(_c.DC,LED,NEO_GRB+NEO_KHZ800);
    NP.begin();
    for(uint16_t n=0;n<_c.DC;n++)
    {
      NP.setPixelColor(n,NP.Color(255,255,255));
      NP.show();
      delay(45);
    }
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
