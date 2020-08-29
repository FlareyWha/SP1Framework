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
	void setFoodlvl(int l);
	void setRentlvl(int l);
	void setShoeslvl(int l);
	void setSCustomerslvl(int l);
	void setRCustomerslvl(int l);
	void setTCustomerslvl(int l);

	void incFoodlvl();
	void incRentlvl();
	void incShoeslvl();
	void incSCustomerslvl();
	void incRCustomerslvl();
	void incTCustomerslvl();
};

