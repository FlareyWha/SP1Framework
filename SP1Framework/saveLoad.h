#pragma once
#include <fstream>
#include <string>  
#include <iostream>
#include <WTypesbase.h>

class saveLoad
{
private:
	int ignore;
	int storedDays;
	int storedMoney;
	int storedSon1;
	int storedSon2;
	int storedPowerups;
	int encryptedPowerup;
	int pShoes;
	int pCrowdcontrol;
	int pCheapfood;
	int pCheaprent;
	int pRichcustomers;
	int pThriftycustomers;

public:
	saveLoad();
	~saveLoad();
	void chooseSave(int save);
	void encryptPowerup(int type, int shoes, int crowdcontrol, int cheapfood,
						int cheaprent, int richcustomers, int thriftycustomers);
	void savePlayerstats(LPCWSTR save, int days, int money, int son1, int son2);
	void loadSave(LPCWSTR save);
	void saveSave(LPCWSTR save);
};