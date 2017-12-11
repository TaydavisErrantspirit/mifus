#include <Arduino.h>

#include "Device.h"

#include "SerialProtocol.h"
#include "Clock.h"
#include "Menu.h"
#include "Memory.h"
#include "Screen.h"

#include "FuckingStupidMenu.h"

  ButtonEnum Device::getKey(unsigned int input)
  {
    int k;
    for (k = 0; k < 5; k++)
    {
      if (input < _prop->adc_key_val[k])
      {
        return (ButtonEnum)k;
      }
    }
    if (k >= 5)
      k = -1;
    return (ButtonEnum)k;
  }

  void Device::parseCommand(SerialProtocol * sp, String data)
  {
  }



    Device::Device(IClock* clock, IMemory* memory, Screen* screen, Properties* prop)
  {
    _clock = clock;
    _memory = memory;
    _screen = screen;
    _prop = prop;
    _sp[0] = new SerialProtocol(new MyHardwareSerial());
    _sp[1] = new SerialProtocol(new MySoftwareSerial(_prop->softrx, _prop->softtx));

    //IMenuNode* node = new TimeMenuNode(clock);
/*
	IMenuNode* nodes[10] = {
		new DebugMenuNode(1, nullptr),//0
	new DebugMenuNode(2, nullptr),		//1
	new DebugMenuNode(3, nullptr),		//2
	new DebugMenuNode(10, nullptr),//3
	new DebugMenuNode(11, nullptr),//4
	new DebugMenuNode(31, nullptr),//5
	new DebugMenuNode(32, nullptr),//6
	new DebugMenuNode(33, nullptr),//7
	new DebugMenuNode(34, nullptr),//8
	new DebugMenuNode(331, nullptr) //9
	};

	nodes[0]->setChild(nodes[3]);

	nodes[3]->setParent(nodes[0]);
	nodes[4]->setParent(nodes[0]);

	nodes[3]->setNext(nodes[4]);
	nodes[4]->setNext(nodes[3]);

	nodes[3]->setPrevious(nodes[4]);
	nodes[4]->setPrevious(nodes[3]);

	nodes[2]->setChild(nodes[5]);

	nodes[5]->setParent(nodes[2]);
	nodes[6]->setParent(nodes[2]);
	nodes[7]->setParent(nodes[2]);
	nodes[8]->setParent(nodes[2]);

	nodes[5]->setNext(nodes[6]);
	nodes[6]->setNext(nodes[7]);
	nodes[7]->setNext(nodes[8]);
	nodes[8]->setNext(nodes[5]);

	nodes[5]->setPrevious(nodes[6]);
	nodes[6]->setPrevious(nodes[7]);
	nodes[7]->setPrevious(nodes[8]);
	nodes[8]->setPrevious(nodes[5]);

	nodes[7]->setChild(nodes[9]);
	nodes[9]->setParent(nodes[7]);

    _menu = new Menu(_screen, nodes[0]);*/
    _menu = new FuckingStupidMenu(_clock,_memory, _screen);
    curentKey = ButtonEnum::NotAkey;
  }

  void Device::begin()
  {
    _memory->begin();
    _clock->begin();
    _screen->begin(_prop->lcd_col, _prop->lcd_row);

    _sp[0]->begin(_prop->rate[0]);
    _sp[1]->begin(_prop->rate[1]);

  }
  IClock* Device::getClock()
  {
    return _clock;
  }
  IMemory* Device::getMemory()
  {
    return _memory;
  }
  Properties* Device::getProperties()
  {
    return _prop;
  }
  Screen* Device::getScreen()
  {
    return _screen;
  }
  void Device::buttonRead()
  {
    if (millis() - previous > 50)
    {
      curentKey = getKey(analogRead(_prop->buttonsPin));
      previous = millis();
    }
    else
    {
      return;
    }
    if (curentKey != NotAkey)
    {
      switch (curentKey)
      {
      case RightKey:
        _menu->right();
        break;
      case UpKey:
        _menu->up();
        break;
      case DownKey:
        _menu->down();
        break;
      case LeftKey:
        _menu->left();
        break;
      case SelectKey:
        _menu->select();
        break;
      default:
        // �� ����� ����
        break;
      }
    }
  }
  void Device::serialInput(int i)
  {
    _sp[i]->Read();
    if (_sp[i]->isDataAviable())
    {
      parseCommand(_sp[i], _sp[i]->getData());

      _sp[i]->ResetAll();
    }
  }
  //  void lcdUpdate();
  void Device::lcdDraw()
  {
    _menu->draw();
  }

  void Device::count() {}
  void Device::sequre() {}

  void Device::update() {}

  bool Device::isSequre()
  {
    return true;
    if (analogRead(_prop->sequrePin) > 600)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

