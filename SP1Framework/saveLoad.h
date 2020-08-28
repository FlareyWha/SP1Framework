#pragma once
#include <fstream>
#include <string>  
#include <iostream>

class saveLoad
{
private:
	int ignore;
	int storedDays;
	int storedMoney;
	int storedSon1;
	int storedSon2;
	int storedPowerups;
	int pShoes;
	int pCrowdcontrol;
	int pCheapfood;
	int pCheaprent;
	int pRichcustomers;
	int pThriftycustomers;
	long long encryptedPowerup;

public:
	saveLoad();
	~saveLoad();
	void chooseSave(int save);
	void encryptPowerup(int shoes, int crowdcontrol, int cheapfood, 
						int cheaprent, int richcustomers, int thriftycustomers);
	void decryptPowerup();
	void loadSave(string save);
	void saveSave(string save);
};