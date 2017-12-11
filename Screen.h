#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class Screen
{
private:
  LiquidCrystal lcd;
public:
  Screen();
  void begin(byte col, byte row);
  void setCursor(byte col, byte row);
  void write(byte data);
  void print(String s);
  void clear();
};
#endif//SCREEN_H
