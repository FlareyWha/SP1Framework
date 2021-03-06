#include "Customer.h"
#include "Position.h"
#include "Windows.h"
#include "Player.h"
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <sstream>

Customer::Customer()//sets which item they want to buy and how much to do so
{
	quantity = 0;
	bSatisfied = true;
	prevPos.setX(0);
	prevPos.setY(0);
	pos.setX(79); //change when we spawn them
	pos.setY(7); // ^
	itemToBuy = 0;
	passcheck = 0;
	CustomerDirection = 0;
	yLock = true;
	avoiding = 0;
	travelling = false;
	spawned = false;
	timerSet = false;
	timer = 0.0;
	movingBack = false;
	colliding = false;
}

Customer::Customer(Player p)
{
	int i = rand() % 100 + 1;
	if (i <= (50 + 7 * (p.getPowerups()->getTCustomerslvl())))
	{
		quantity = 1;
	}
	else
	{
		quantity = 2;
	}
	bSatisfied = true;
	prevPos.setX(0);
	prevPos.setY(0);
	pos.setX(79); //change when we spawn them
	pos.setY(7); // ^
	itemToBuy = 0;
	passcheck = 0;
	CustomerDirection = 0;
	yLock = true;
	avoiding = 0;
	travelling = false;
	spawned = false;
	timerSet = false;
	timer = 0.0;
	movingBack = false;
	colliding = false;
}

Customer::~Customer()
{

}

void Customer::moveToShelfContainingItem(int itemToBuy)//brings them to the shelf depending on what item they are buying
{
	switch (itemToBuy)
	{
	case 1:
		endPoint.setX(37); //change when we spawn them
		endPoint.setY(7); // ^
		break;
	case 2:
		endPoint.setX(37); //change when we spawn them
		endPoint.setY(13); // ^
		break;
	case 3:
		endPoint.setX(37); //change when we spawn them
		endPoint.setY(19); // ^
		break;
	case 4:
		endPoint.setX(58); //change when we spawn them
		endPoint.setY(7); // ^
		break;
	case 5:
		endPoint.setX(58); //change when we spawn them
		endPoint.setY(13); // ^
		break;
	case 6:
		endPoint.setX(58); //change when we spawn them
		endPoint.setY(19); // ^
		break;
	}
}

bool Customer::entranceWaiting(void)
{
	//after 10 seconds trigger this function to set it to true
	return true;
}

bool Customer::shelfWaiting(void)
{
	//after 20 seconds trigger this function to set it to true
	return true;
}

void Customer::isSatisfied()
{
	bSatisfied = true;
}

void Customer::unSatisfied()
{
	bSatisfied = false;
}

bool Customer::getState()
{
	return bSatisfied;
}

bool Customer::getTravelling()
{
	return travelling;
}

bool Customer::getMovingBack()
{
	return movingBack;
}

bool Customer::getTimerSet()
{
	return timerSet;
}

double Customer::getTimer()
{
	return timer;
}

bool Customer::getColliding()
{
	return colliding;
}

Position Customer::getPrevPos()
{
	return prevPos;
}

int Customer::getAvoiding()
{
	return avoiding;
}

void Customer::setItemToBuy(int passcheck)
{
	itemToBuy = rand() % passcheck + 1;
}

void Customer::setAvoiding(int number)
{
	this->avoiding = number;
}

void Customer::setTravelling(bool travelling)
{
	this->travelling = travelling;
}

void Customer::setSpawned(bool spawned)
{
	this->spawned = spawned;
}

void Customer::setMovingBack(bool movingBack)
{
	this->movingBack = movingBack;
}

void Customer::setTimer(double timer)
{
	this->timer = timer;
}

void Customer::addTimer(double dt)
{
	timer += dt;
}

void Customer::setTimerSet(bool timerSet)
{
	this->timerSet = timerSet;
}

int Customer::getQuantity()
{
	return quantity;
}

int Customer::getItemToBuy()
{
	return itemToBuy;
}

void Customer::printOutCustomer(Console& console, Map& map, int q, WORD s)
{
	COORD c;
	if (spawned == true)
	{
		std::ostringstream ss;
		ss.str("");
		ss << q;
		c.X = pos.getX();
		c.Y = pos.getY();
		
		if (bSatisfied == false)
		{
			console.writeToBuffer(c,' ', s);
		}
		else if (q == 0)
		{
			console.writeToBuffer(c, 1, s);
		}
		else
		{
			console.writeToBuffer(c, ss.str(), s);
		}		
		map.setGrid(pos.getY(), pos.getX(), 'C');
	}
}

int Customer::moveCustomer(Map& map, int framesPassed, int timer)
{
	if (framesPassed % timer == 0)
	{
		prevPos.setX(pos.getX());
		prevPos.setY(pos.getY());

		if ((endPoint.getY() - pos.getY()) != 0 && yLock == false)
		{
			if (map.isNegative(endPoint.getY(), pos.getY()) == true) //changed in attempt to fix collision
			{
				//if (map.getGrid(pos.getY() - 1, pos.getX()) == 'C') //CHANGE THIS FOR COLLISION WITH CUSTOMERS ON Y AXIS (NORTH
				//{
					//pos.setY(pos.getY() - 1);
					//return 1;
				//}
				if (map.getGrid(pos.getY() - 1, pos.getX()) == '0')
				{
					pos.setY(pos.getY() - 1);
					return 1;
				}
			}
			else if (map.isNegative(endPoint.getY(), pos.getY()) == false)
			{
				//if (map.getGrid(pos.getY() + 1, pos.getX()) == 'C') //CHANGE THIS FOR COLLISION WITH CUSTOMERS ON Y AXIS (SOUTH
				//{
					//pos.setY(pos.getY() + 1);
					//return 1;
				//}
				if (map.getGrid(pos.getY() - 1, pos.getX()) == '0')
				{
					pos.setY(pos.getY() + 1);
					return 3;
				}
			}
		}
		if ((endPoint.getX() - pos.getX()) != 0)
		{
			if (map.isNegative(endPoint.getX(), pos.getX()) == true)
			{
				if (map.getGrid(pos.getY(), pos.getX() - 3) == 'C')
				{
					pos.setX(pos.getX());
					colliding = true;
				}
				else if (map.getGrid(pos.getY(), pos.getX() - 1) == '0')
				{
					pos.setX(pos.getX() - 1);
					colliding = false;
					return 2;
				}
			}

			else if (map.isNegative(endPoint.getX(), pos.getX()) == false)
			{
				//if (map.getGrid(pos.getY(), pos.getX() + 3) == 'C')
				//{
					//pos.setX(pos.getX());
					//colliding = true;
				//}
				if (map.getGrid(pos.getY(), pos.getX() + 1) == '0')
				{
					pos.setX(pos.getX() + 1);
					colliding = false;
					return 0;
				}
			}
		}
		map.setGrid(prevPos.getY(), prevPos.getX(), '0');
		map.setGrid(pos.getY(), pos.getX(), 'C');
	}
} 

void Customer::customerCollision(Map& map)
{
	if ((map.getGrid(pos.getY(), pos.getX() - 3) == 'C') && (pos.getX() - endPoint.getX() > 19))
	{
		travelling = false;
		avoiding = 1;
	}

	else if (avoiding == 3 && map.getGrid(pos.getY() - 1, pos.getX()) == 'C')
	{
		avoiding = 2;
	}

	if (avoiding == 1)
		pos.setY(pos.getY() + 1);
	else if (avoiding == 2)
		pos.setX(pos.getX() - 3);
	else if (avoiding == 3)
	{
		pos.setY(pos.getY() - 1);
	}
	else if (avoiding == 5) // to move down after collecting item
	{
		pos.setY(pos.getY() + 1);
		yLock = true;
	}
	else if (avoiding == 6)
	{
		pos.setY(pos.getY() + 1);
		travelling = true;
	}

	if (avoiding == 4 || avoiding == 7)
	{
		avoiding = 0;
		travelling = true;
	}

	if (avoiding != 0)
	{
		avoiding++;
	}
}

Position Customer::getEndPoint()
{
	return endPoint;
}

void Customer::setEndPoint(int x, int y)
{
	endPoint.setX(x);
	endPoint.setY(y);
}

void Customer::setYLock(bool set)
{
	yLock = set;
}

int Customer::getEndPointX()
{
	return endPoint.getX();
}

int Customer::getEndPointY()
{
	return endPoint.getY();
}

void Customer::setQuantity(int quantity)
{
	this->quantity = quantity;
}

int Customer::getX() {
	return pos.getX();
}

int Customer::getY() {
	return pos.getY();
}

