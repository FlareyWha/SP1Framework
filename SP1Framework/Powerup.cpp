#include "Powerup.h"

Powerup::Powerup()
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
