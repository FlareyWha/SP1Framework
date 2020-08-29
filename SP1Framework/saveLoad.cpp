#include "saveLoad.h"

using namespace std;

saveLoad::saveLoad()
{
}

saveLoad::~saveLoad()
{
}

void saveLoad::chooseSave(int save) //maybe useful
{
    switch (save)
    {
    case 0:
        {
            fstream save1("save1");
            break;
        }
    case 1:
        {
            fstream save2("save2");
            break;
        }
    }
}

void saveLoad::encryptPowerup(int shoes, int crowdcontrol, int cheapfood,
    int cheaprent, int richcustomers, int thriftycustomers)
{
    for (int i = 0; i < shoes; i++)
    {
        encryptedPowerup = encryptedPowerup * 2;
    }
    for (int i = 0; i < crowdcontrol; i++)
    {
        encryptedPowerup = encryptedPowerup * 3;
    }
    for (int i = 0; i < cheapfood; i++)
    {
        encryptedPowerup = encryptedPowerup * 5;
    }
    for (int i = 0; i < cheaprent; i++)
    {
        encryptedPowerup = encryptedPowerup * 7;
    }
    for (int i = 0; i < richcustomers; i++)
    {
        encryptedPowerup = encryptedPowerup * 11;
    }
    for (int i = 0; i < thriftycustomers; i++)
    {
        encryptedPowerup = encryptedPowerup * 13;
    }
}

void saveLoad::decryptPowerup()
{
    while (encryptedPowerup > 1)
    {
        if (encryptedPowerup % 13 == 0)
        {
            pThriftycustomers++;
            encryptedPowerup = encryptedPowerup / 13;
        }
        else if (encryptedPowerup % 11 == 0)
        {
            pRichcustomers++;
            encryptedPowerup = encryptedPowerup / 11;
        }
        else if (encryptedPowerup % 7 == 0)
        {
            pCheaprent++;
            encryptedPowerup = encryptedPowerup / 7;
        }
        else if (encryptedPowerup % 5 == 0)
        {
            pCheapfood++;
            encryptedPowerup = encryptedPowerup / 5;
        }
        else if (encryptedPowerup % 3 == 0)
        {
            pCrowdcontrol++;
            encryptedPowerup = encryptedPowerup / 3;
        }
        else if (encryptedPowerup % 2 == 0)
        {
            pShoes++;
            encryptedPowerup = encryptedPowerup / 2;
        }
    }
}

void saveLoad::savePlayerstats(int days, int money, int son1, int son2)
{
    fstream myFile;

    myFile.open("save1.txt");
    if (myFile.is_open())
    {
        myFile >> ignore >> storedDays >> storedMoney >> storedSon1 >> storedSon2 >> storedPowerups;
        myFile << "Days = " << days << endl;
        myFile << "Money = " << money << endl;
        myFile << "son1Sick = " << son1 << endl;
        myFile << "son2Sick = " << son2 << endl;
        myFile.close();
    }
}

void saveLoad::loadSave(LPCWSTR save) //DOES NOT WORK, HOW THE FUCK DO YOU CALL A .TXT 
{
    fstream myFile;

    myFile.open(save);
    if (myFile.is_open())
    {
        myFile >> ignore >> storedDays >> storedMoney >> storedSon1  >> storedSon2 >> storedPowerups;

        myFile.close();
    }

    cout << storedDays << endl;
    cout << storedMoney << endl;
    cout << storedSon1 << endl;
    cout << storedSon2 << endl;
    cout << storedPowerups << endl;
    myFile.close();
}

void saveLoad::saveSave(LPCWSTR save) //DOES NOT WORK, HOW THE FUCK DO YOU CALL A .TXT 
{
    fstream myFile;

    myFile.open(save);
    if (myFile.is_open())
    {
        myFile >> ignore >> storedDays >> storedMoney >> storedSon1 >> storedSon2 >> storedPowerups;

        myFile.close();
    }

    myFile << "Days = " << storedDays << endl;
    myFile << "Money = " << storedMoney << endl;
    myFile << "son1Sick = " << storedSon1 << endl;
    myFile << "son2Sick = " << storedSon2 << endl;
    myFile << "storedPowerups = " << storedPowerups << endl;
}
