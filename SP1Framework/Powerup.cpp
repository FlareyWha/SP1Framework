#include "Powerup.h"

Powerup::Powerup() : cheaperFood(0), cheaperRent(0), playerShoes(0), slowerCustomers(0), richCustomers(0), thriftyCustomers(0)
{
}

Powerup::~Powerup()
{
}

int Powerup::getFoodlvl()
{
	return cheaperFood;
}

int Powerup::getRentlvl()
{
	return cheaperRent;
}

int Powerup::getShoeslvl()
{
	return playerShoes;
}

int Powerup::getSCustomerslvl()
{
	return slowerCustomers;
}

int Powerup::getRCustomerslvl()
{
	return richCustomers;
}

int Powerup::getTCustomerslvl()
{
	return thriftyCustomers;
}

void Powerup::purchasecheaperFood()
{
	if (cheaperFood < 3) {
		cheaperFood++;
	}
}

void Powerup::purchasecheaperRent()
{
	if (cheaperRent < 3) {
		cheaperRent++;
	}
}

void Powerup::purchaseplayerShoes()
{

}

void Powerup::purchaseslowerCustomers()
{
	
}

void Powerup::purchaserichCustomers()
{
	
}

void Powerup::purchasethriftyCustomers()
{
	
}
