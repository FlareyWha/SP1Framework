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
	return getShoeslvl();
}

int Powerup::getSCustomerslvl()
{
	return getSCustomerslvl();
}

int Powerup::getRCustomerslvl()
{
	return getRCustomerslvl();
}

int Powerup::getTCustomerslvl()
{
	return getTCustomerslvl();
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
	if (playerShoes < 3) {
		playerShoes++;
	}
}

void Powerup::purchaseslowerCustomers()
{
	if (slowerCustomers) {

	}
}

void Powerup::purchaserichCustomers()
{
	
}

void Powerup::purchasethriftyCustomers()
{
	
}
