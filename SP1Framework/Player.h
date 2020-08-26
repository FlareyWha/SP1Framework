#pragma once
#include "Entity.h"
#include "Framework/console.h"

class Player :
	public Entity 
{
private:
	int strikes;
	int savings; 

	int unsatisfiedCustomers;

	int dayEarnings;

	bool holdingProduct;
	bool p_bRentPaid;

public:
	Player();
	~Player();

	int getStrikes(); // return strikes
	int getSavings(); // return savings

	int getTotalEarned();
	void AddDayEarnings(int i);
	void resetDayEarnings();

	int getUnsatisfiedCustomers();
	void increaseUnsatisfiedCustomers();
	void resetUnsatisfiedCustomers();

	void receiveStrike(); //set strikes, strikes++
	void receivePay(int pay); //set savings, increase savings

	void payRent(); // rent cost = 200 
	void payFood(); // food cost = 30
	void payMedicine(); // medicine cost = 100
	void isRentPaid();
	bool getRentStatus();

	void holdsProduct(); //set holdingProduct = true
	bool isHoldingProduct(); //get holdingProduct
	void releaseProduct(); //set holdingProduct = false

};

