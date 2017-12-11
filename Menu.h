#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "Device.h"

//class Device;
//struct Properties;
//class Screen;
#include "Screen.h"
//class Clock;
#include "Clock.h"
//class Memory;
#include "Memory.h"

#include "IList.h"

class IMenuNode :public INode
{
public:
	IMenuNode(INode* parent) :INode(parent)
	{}
	IMenuNode() :IMenuNode(nullptr)
	{}

	virtual void draw(Screen* lcd) = 0;

	virtual IMenuNode* left() = 0;
	virtual IMenuNode* right() = 0;
	virtual IMenuNode* up() = 0;
	virtual IMenuNode* down() = 0;

	virtual IMenuNode* select() = 0;

};

//Текущее время
class TimeMenuNode:public IMenuNode
{
private:
	IClock* _clock;
public:
	TimeMenuNode(IClock* clock, INode* parent):IMenuNode(parent)
	{
		_clock = clock;
	}
	TimeMenuNode(IClock* clock) :TimeMenuNode(clock, nullptr)
	{}
	
	virtual void draw(Screen* lcd)
	{
		DateTime now = _clock->now();
		String data = "";
		if (now.hour() < 10)
		{
			data += "0";
		}
		data += String(now.hour());
		if ((now.second() % 2) == 0)
		{
			data += ":";
		}
		else
		{
			data += " ";
		}
		if (now.minute() < 10)
		{
			data += "0";
		}
		data += String(now.minute()) + " ";
		if (now.day() < 10)
		{
			data += "0";
		}
		data += String(now.day()) + ".";
		if (now.month() < 10)
		{
			data += "0";
		}
		data += String(now.month()) + ".";
		data += String(now.year());


		lcd->print("0,1");
		lcd->setCursor(0, 1);
		lcd->print(data);
	}
	virtual IMenuNode* left()
	{
		return this;
	}
	virtual IMenuNode* right()
	{
		return this;
	}
	virtual IMenuNode* up()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Up));
	}
	virtual IMenuNode* down()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Down));
	}
	virtual IMenuNode* select()
	{
		return right();
	}
	~TimeMenuNode()
	{
		_clock = nullptr;
	}
};

class CounterDataNode:public IMenuNode
{
private:
	IMemory* _memory;
	byte _counterID;
public:
	CounterDataNode(IMemory* memory, INode* parent):IMenuNode(parent)
	{
		_memory = memory;
		_counterID = 0;
	}
	CounterDataNode(IMemory* memory) :CounterDataNode(memory, nullptr)
	{

	}
	~CounterDataNode()
	{

	}

	virtual void draw(Screen* lcd)
	{
		lcd->print("DAHHI ");
		lcd->write(7);
		lcd->print("I4U");
		lcd->write(7);
		lcd->print("bHUKA:");

		lcd->setCursor(0, 1);
		lcd->print(String(_memory->getCurent(_counterID)));
	}
	
	virtual IMenuNode* left()
	{
		_counterID = (_counterID == 0) ? 1 : 0;
		return this;
	}
	virtual IMenuNode* right()
	{
		_counterID = (_counterID == 0) ? 1 : 0;
		return this;
	}
	virtual IMenuNode* up()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Up));
	}
	virtual IMenuNode* down()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Down));
	}
	virtual IMenuNode* select()
	{
		return right();
	}
};

class DebugMenuNode:public IMenuNode
{
private:
	int a;
public:
	DebugMenuNode(int i, INode* parent) :IMenuNode(parent)
	{
		a = i;
	}
	virtual IMenuNode* left()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Left));
	}
	virtual IMenuNode* right()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Right));
	}
	virtual IMenuNode* up()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Up));
	}
	virtual IMenuNode* down()
	{
		return reinterpret_cast<IMenuNode*>(getNeighbor(Down));
	}
	virtual IMenuNode* select()
	{
		return right();
	}

	virtual void draw(Screen* screen)
	{
		screen->print("debug node ");
		screen->print(String(a));
	}
};
class Menu:public IList
{
//	Device* _device;
	Screen* _screen;
public:
	/*Menu(Device* device, IMenuNode* node) :IList(node)
	{
		_device = device;
	}
	Menu(Device* device) :Menu(device, nullptr)
	{}*/

	Menu(Screen* screen, IMenuNode* node) :IList(node)
	{
		_screen = screen;
	}
	Menu(Screen* screen):Menu(screen, nullptr){}

	void draw()
	{
		if (getCurent())
		{
			reinterpret_cast<IMenuNode*>(getCurent())->draw(_screen);
		}
	}

	void left()
	{
		setCurent(reinterpret_cast<IMenuNode*>(getCurent())->left());
		_screen->clear();
	}
	void right()
	{
		setCurent(reinterpret_cast<IMenuNode*>(getCurent())->right());
		_screen->clear();
	}
	void up()
	{
		setCurent(reinterpret_cast<IMenuNode*>(getCurent())->up());
		_screen->clear();
	}
	void down()
	{
		setCurent(reinterpret_cast<IMenuNode*>(getCurent())->down());
		_screen->clear();
	}

	void select()
	{
		setCurent(reinterpret_cast<IMenuNode*>(getCurent())->select());
	}
};
#endif //MENU_H
