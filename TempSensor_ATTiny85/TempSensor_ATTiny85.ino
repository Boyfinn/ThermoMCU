/*
PacketSize:
|   #0  |#1|#2|#3|#4|#5|#6|   #7  |   #8   |   #9  | #10  |
|Command|LR|LG|LB|HR|HG|HB|LowTemp|HighTemp|HIGHLed|LowLED|
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sysSerial.h"

#define BUS 5

//Dallas OneWireBus PB2; //ATtiny85 (BUS)
OneWire OW(BUS);
DallasTemperature DS(&OW);

void setup() {
  SerialSetup();
  LoadEEPROM();
  if(DS.getDS18Count() != 0)
    DS.setResolution(12);
}
void loop() {
  GetSerial();
  if(DS.getDS18Count() != 0) {
    DS.requestTemperatures();
    double temp = DS.getTempCByIndex(0);
  }
  delay(1000);
}
