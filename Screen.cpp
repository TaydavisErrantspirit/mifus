#include "Screen.h"
#include <LiquidCrystal.h>

  Screen::Screen():lcd(8, 9, 4, 5, 6, 7)
  {
    byte customChars[][8] = {
      { 0b11110,0b10010,0b10000,0b10000,0b10000,0b10000,0b10000,0b00000 },    //�
      { 0b01110,0b10101,0b10101,0b01110,0b00100,0b00100,0b00100,0b00000 },    //�
//      {0b10101,0b10101,0b10101,0b01110,0b10101,0b10101,0b10101,0b00000 },     //�
      { 0b10010,0b10010,0b10010,0b10010,0b10010,0b10010,0b11111,0b00001 },    //�
      { 0b00111,0b01001,0b01001,0b01001,0b01001,0b01001,0b10001,0b00000 },    //�
      { 0b10001,0b10001,0b10101,0b10101,0b10101,0b10101,0b11111,0b00000 },    //�
      { 0b10010,0b10101,0b10101,0b11101,0b10101,0b10101,0b10010,0b00000 },    //�
      { 0b11111,0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b00000 },    //�
      { 0b01111,0b10001,0b10001,0b01111,0b00101,0b01001,0b10001,0b00000 } };    //�
    for (byte i = 0; i < 8; ++i)
    {
      lcd.createChar(i, customChars[i]);
    }
  }
  void Screen::begin(byte col, byte row)
  {
    lcd.begin(col, row);
    lcd.clear();
  }
  void Screen::setCursor(byte col, byte row)
  {
    lcd.setCursor(col, row);
  }
  void Screen::write(byte data)
  {
    lcd.write(data);
  }
  void Screen::print(String s)
  {
    lcd.print(s);
  }
  void Screen::clear()
  {
    lcd.clear();
  }
