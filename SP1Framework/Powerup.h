#pragma once
class Powerup
{
private:
	int cheaperFood; //Max: 3
	int cheaperRent; //Max: 3
	int playerShoes; //Max: 3
	int slowerCustomers; // Max: 3
	int richCustomers; // Max: 3
	int thriftyCustomers; // Max: 3

public:
	Powerup();
	~Powerup();

	int purchasecheaperFood(); //cheaper food
	int purchasecheaperRent(); //cheaper rent
	int purchaseplayerShoes(); //faster player
	int purchaseslowerCustomers(); //slower customers
	int purchaserichCustomers(); //more money per customer
	int purchasethriftyCustomers(); //less chance of buying >1 item
};

