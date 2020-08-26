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

	void resetall(); //reset levels

	int getFoodlvl(); //get current level of food
	int getRentlvl(); //get current level of rent
	int getShoeslvl(); //get current level of shoes
	int getSCustomerslvl(); //get current level of slowerCustomers
	int getRCustomerslvl(); //get current level of richCustomers
	int getTCustomerslvl(); //get current level of thriftyCustomers

	void purchasecheaperFood(); //cheaper food
	void purchasecheaperRent(); //cheaper rent
	void purchaseplayerShoes(); //faster player
	void purchaseslowerCustomers(); //slower customers
	void purchaserichCustomers(); //more money per customer
	void purchasethriftyCustomers(); //less chance of buying >1 item
};

