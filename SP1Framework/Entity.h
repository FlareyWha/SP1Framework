#pragma once
#include "Position.h"
class Entity
{
protected:
	Position pos;

public:
	Entity();
	virtual ~Entity();
	Position getPos(void);
	void setPos(int x, int y);
};

