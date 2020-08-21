#include "Customer.h"
#include "Position.h"
#include "Windows.h"
#include <stdlib.h>

Customer::Customer()//sets which item they want to buy and how much to do so
{
	//itemToBuy = rand() % 6 + 1; // 1 = rice, 2 = toilet paper, 3 = canned food, 4 = instant noodles, 5 = vegetables, 6 = bandages
	quantity = rand() % 10 + 1;
	bSatisfied = false;
	prevPos.setX(0);
	prevPos.setY(0);
	pos.setX(79); //change when we spawn them
	pos.setY(13); // ^
}

//Customer::Customer(Map map)
//{
//	int x, y;
//	bool spawnComplete = false;
//	while (spawnComplete == false)
//	{
//		
//	}
//}

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
		endPoint.setX(0); //change when we spawn them
		endPoint.setY(0); // ^
		break;
	case 4:
		endPoint.setX(0); //change when we spawn them
		endPoint.setY(0); // ^
		break;
	case 5:
		endPoint.setX(0); //change when we spawn them
		endPoint.setY(0); // ^
		break;
	case 6:
		endPoint.setX(0); //change when we spawn them
		endPoint.setY(0); // ^
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

void Customer::setItemToBuy(int passcheck)
{
	itemToBuy = rand() % passcheck + 1;
}

int Customer::getQuantity()
{
	return quantity;
}

int Customer::getItemToBuy()
{
	return itemToBuy;
}

void Customer::printOutCustomer(bool spawned, Console& console, Position pos, Map& map)
{
	COORD c;
	if (spawned == true)
	{
		c.X = pos.getX();
		c.Y = pos.getY();
		console.writeToBuffer(c, ' ', 0x20);
		map.setGrid(c.Y, c.X, 'C');
	}
}

void Customer::moveCustomer(Map& map)
{
	prevPos.setX(pos.getX());
	prevPos.setY(pos.getY());

	if ((endPoint.getY() - pos.getY()) != 0)
	{
		if (map.isNegative(endPoint.getY(), pos.getY()) == true)
		{
			if (map.getGrid(pos.getY() - 1, pos.getX()) == '0')
				pos.setY(pos.getY() - 1);
			else
				pos.setY(pos.getY());
		}
		if (map.isNegative(endPoint.getY(), pos.getY()) == false)
		{
			if (map.getGrid(pos.getY() + 1, pos.getX()) == '0')
				pos.setY(pos.getY() + 1);
			else
				pos.setY(pos.getY());
		}
	}
	if ((endPoint.getX() - pos.getX()) != 0)
	{
		if (map.isNegative(endPoint.getX(), pos.getX()) == true)
		{
			if (map.getGrid(pos.getY(), pos.getX() - 1) == '0')
				pos.setX(pos.getX() - 1);
			else
				pos.setX(pos.getX());
		}

		if (map.isNegative(endPoint.getX(), pos.getX()) == false)
		{
			if (map.getGrid(pos.getY(), pos.getX() + 1) == '0')
				pos.setX(pos.getX() + 1);
			else
				pos.setX(pos.getX());
		}
	}

	map.setGrid(prevPos.getY(), prevPos.getX(), '0');
}

void Customer::bumpIntoCustomer(int avoiding, Map& map)
{
	// avoiding 1-3 is for if the moving customer is on the right of a still customer
	// avoiding 4-6 is for if the moving customer is on the left of a still customer
	if ((avoiding == 1) || (avoiding == 6))
		pos.setY(pos.getY() + 1);
	else if (avoiding == 2)
		pos.setX(pos.getX() - 2);
	else if (avoiding == 4)
		pos.setX(pos.getX() + 2);
	else if ((avoiding == 3) || (avoiding == 5))
		pos.setY(pos.getY() - 1);
}



