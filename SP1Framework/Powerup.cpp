#include "Powerup.h"

Powerup::Powerup() : cheaperFood(0), cheaperRent(0), playerShoes(0), slowerCustomers(0), richCustomers(0), thriftyCustomers(0)
{
}

Powerup::~Powerup()
{
}

void Powerup::resetall()
{
	cheaperFood = 0;
	cheaperRent = 0;
	playerShoes = 0;
	slowerCustomers = 0;
	richCustomers = 0;
	thriftyCustomers = 0;
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

void Powerup::setFoodlvl(int l)
{
	cheaperFood = l;
}

void Powerup::setRentlvl(int l)
{
	cheaperRent = l;
}

void Powerup::setShoeslvl(int l)
{
	playerShoes = l;
}

void Powerup::setSCustomerslvl(int l)
{
	slowerCustomers = l;
}

void Powerup::setRCustomerslvl(int l)
{
	richCustomers = l;
}

void Powerup::setTCustomerslvl(int l)
{
	thriftyCustomers = l;
}

void Powerup::incFoodlvl()
{
	cheaperFood++;
}

void Powerup::incRentlvl()
{
	cheaperRent++;
}

void Powerup::incShoeslvl()
{
	playerShoes++;
}

void Powerup::incSCustomerslvl()
{
	slowerCustomers++;
}

void Powerup::incRCustomerslvl()
{
	richCustomers++;
}

void Powerup::incTCustomerslvl()
{
	thriftyCustomers++;
}
