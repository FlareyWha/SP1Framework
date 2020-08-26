#pragma once
class Powerup
{
public:
	Powerup();
	~Powerup();

	int cheaperFood(); //cheaper food
	int cheaperRent(); //cheaper rent
	int playerShoes(); //faster player
	int slowerCustomers(); //slower customers
	int richCustomers(); //more money per customer
	int thriftyCustomers(); //less chance of buying >1 item
};

