#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

class IClock;
class Menu;
class IMemory;
class Screen;
class SerialProtocol;
class Memory;


struct Properties
{
  int softrx;
  int softtx;
  int rate[2] = { 9600, 9600 };

  byte buttonsPin = 0;
  byte sequrePin = 1;
  byte butteryPin = 2;

  byte counters = 2;

  byte counterPins[1] = { 2 };
  byte sequrePins[1] = { 3 };

  int lcd_i2c_adrr = 0x27;//���������
  byte lcd_col = 16;
  byte lcd_row = 2;

  int adc_key_val[5] = { 30, 160, 360, 535, 760 };

};
enum ButtonEnum
{
  RightKey,
  UpKey,
  DownKey,
  LeftKey,
  SelectKey,
  LastKey = SelectKey,

  NotAkey = 255
};

class Device
{
private:
	IClock* _clock;
	IMemory* _memory;
	Screen* _screen;
	Properties* _prop;
	Menu* _menu;

	SerialProtocol* _sp[2];

	ButtonEnum curentKey;
	long previous = 0;

	ButtonEnum getKey(unsigned int input);
	void parseCommand(SerialProtocol* sp, String data);
public:
	Device(IClock* clock, Memory* memory, Screen* screen, Properties* prop);
	void begin();	
	IClock* getClock();	
	IMemory* getMemory();	
	Properties* getProperties();	
	Screen* getScreen();	
	void buttonRead();	
	void serialInput(int i);	
	void lcdDraw();

	void count();
	void sequre();

	void update();

	bool isSequre();
};
#endif //DEVICE_H
