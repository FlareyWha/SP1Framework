#pragma once
#include "Entity.h"
#include "Powerup.h"
#include "Son.h"
#include "Framework/console.h"

class Player :
	public Entity 
{
private:
	int strikes;
	int savings; 
	Powerup* PowerupsPtr;

	int unsatisfiedCustomers;

	int dayEarnings;

	bool holdingProduct;
	bool p_bRentPaid;

	int rent, food, medicine;

public:
	Player();
	~Player();

	Powerup* getPowerups();

	int getStrikes(); // return strikes
	int getSavings(); // return savings
	void setSavings(int savings);

	int getTotalEarned();
	void AddDayEarnings(int i);
	void resetDayEarnings();

	int getUnsatisfiedCustomers();
	void increaseUnsatisfiedCustomers();
	void resetUnsatisfiedCustomers();

	void receiveStrike(); //set strikes, strikes++
	void receivePay(int pay); //set savings, increase savings
	void takePay(int pay);

	void payRent(); // rent cost = 200 
	void payFood(Son* cPtr); // food cost = 30
	void payMedicine(Son* cPtr); // medicine cost = 100
	void isRentPaid();
	bool getRentStatus();

	void holdsProduct(); //set holdingProduct = true
	bool isHoldingProduct(); //get holdingProduct
	void releaseProduct(); //set holdingProduct = false

	void setRent(int r);
	void setFood(int f);
	void setMedicine(int m);
	int getRent();
	int getFood();
	int getMedicine();

	void purchasecheaperFood(Player* p); //cheaper food
	void purchasecheaperRent(Player* p); //cheaper rent
	void purchaseplayerShoes(Player* p); //faster player
	void purchaseslowerCustomers(Player* p); //slower customers
	void purchaserichCustomers(Player* p); //more money per customer
	void purchasethriftyCustomers(Player* p); //less chance of buying >1 item
};

