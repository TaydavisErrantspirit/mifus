#include <RTClib.h>
#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>

#include "Clock.h"
#include "Device.h"
#include "Memory.h"
#include "Screen.h"
#include "SerialProtocol.h"

IClock* clock;
Memory* memory;
Device* device;
Screen* screen;

Properties prop;

unsigned long int previous;
void setup() {
  
  clock = new Clock();
  memory = new Memory();
  screen = new Screen();
  device = new Device(clock, memory, screen, &prop);
  
  device->begin();

  previous = millis();
}

long int delta;
void loop() {
  long curent = millis();
  delta = curent - previous;
  previous = curent;
  if(curent<previous)
  {
    previous = curent;
    return;
  }
  previous = curent;
  
  device->buttonRead();
  device->serialInput(0);
  
  device->lcdDraw();

  device->sequre();

  
}
