// IList.h

#ifndef _ILIST_h
#define _ILIST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

enum Direction
{
	Left = 0,
	Up = 1,
	Right = 2,
	Down = 3
};

class INode
{
	INode* _neighbors[4];

	byte isDeleting = 0;
	static void clear(INode* node)
	{
		node->isDeleting = 1;
		for (byte i = 3; i > 1; ++i)
		{
			if (node->_neighbors[i] && node->_neighbors[i]->isDeleting)
			{
				clear(node->_neighbors[i]);
			}
		}
		delete(node);
	}
public:
	INode()
	{
		for (byte i = 0; i < 4; ++i)
		{
			_neighbors[i] = nullptr;
		}
	}
	INode(INode* parent) :INode()
	{
		setParent(parent);
	}

	void setNeighbor(Direction direction, INode* node)
	{
		_neighbors[direction] = node;
		if (direction > 1)
		{
			node->setNeighbor(Direction(direction - 2), this);
		}
		else
		{
			node->setNeighbor(Direction(direction + 2), this);
		}
	}

	INode* getNeighbor(Direction direction)
	{
		return _neighbors[direction];
	}


	void setParent(INode* node)
	{
		setNeighbor(Left, node);
	}
	void setChild(INode* node)
	{
		setNeighbor(Right, node);
	}
	void setPrevious(INode* node)
	{
		setNeighbor(Up, node);
	}
	void setNext(INode* node)
	{
		setNeighbor(Down, node);
	}

	INode*getParent(Direction direction)
	{
		return getNeighbor(direction);
	}
	INode*getChild(Direction direction)
	{
		return getNeighbor(direction);
	}
	INode*getPrevious(Direction direction)
	{
		return getNeighbor(direction);
	}
	INode*getNext(Direction direction)
	{
		return getNeighbor(direction);
	}

	void clear()
	{
		clear(this);
	}
};
class IList
{
	INode* _root;
	INode* _curent;
public:
	IList(INode* node)
	{
		_root = node;
		_curent = _root;
	}
	IList():IList(nullptr)
	{}

	void setCurent(INode* node)
	{
		if (node)
		{
			_curent = node;
		}
	}
	INode* getCurent()
	{
		return _curent;
	}
	void setRoot(INode* node)
	{
		if (_root)
		{
			_root->clear();
			_root = nullptr;
		}
		_root = node;
	}

	~IList()
	{
		if (_root)
		{
			_root->clear();
			_root = nullptr;
		}
	}
};
#endif

