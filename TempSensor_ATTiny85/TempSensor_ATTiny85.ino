/*
PacketSize:
|   #0  |#1|#2|#3|#4|#5|#6|   #7  |   #8   |   #9  | #10  |
|Command|LR|LG|LB|HR|HG|HB|LowTemp|HighTemp|HIGHLed|LowLED|
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DS18B20_INT.h>
#include <FastLED.h>
#include "sysSerial.h"

#define numLED 512

#define BUS 2
#define LED 3
#define TCL 8

//Dallas OneWireBus PB2; //ATtiny85 (BUS)
OneWire OW(BUS);
DS18B20_INT DS(&OW);
CRGB DiodeStrip[numLED];
CRGB hiCol;
CRGB loCol;
bool heartbeat = false;

void setup() {
  Serial.begin(115200);
  Serial.println("lol");
  pinMode(TCL, INPUT);
  SerialSetup();
  LoadEEPROM();
  DS.begin();
  if(_c.DC <= 512)
  {
    FastLED.addLeds<WS2812B, LED, GRB>(DiodeStrip, numLED);
    hiCol = CRGB( _c.HR, _c.HG, _c.HB);
    loCol = CRGB( _c.LR, _c.LG, _c.LB);
    for(uint16_t n=0;n<_c.DC;n++)
    {
      DiodeStrip[n] = loCol;
      FastLED.show();
    }
    for(uint16_t n=0;n<_c.DC;n++)
    {
      DiodeStrip[n] = hiCol;
      FastLED.show();
    }
  }
  DS.setResolution(9);
}
void loop() {
  if(digitalRead(TCL))
  {
    GetSerial();
    return;
  }
  DS.requestTemperatures();
  int t = DS.getTempC();
  if(t <=_c.TH && t >= _c.TL)
  {
    int m = map(t,_c.TL, _c.TH,0,_c.DC);
    int range = map(t,_c.TL, _c.TH,0,255);
    CRGB c = blend(loCol,hiCol,range);
    for(uint16_t n=0;n<_c.DC;n++)
    {
      DiodeStrip[n] = (n <= m)?c:CRGB::Black;
      FastLED.show();
    }
  }
  digitalWrite(LED_BUILTIN, heartbeat = !heartbeat);
  Serial.println(t);
}
