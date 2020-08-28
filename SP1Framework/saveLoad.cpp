#include "saveLoad.h"
#include <fstream>
#include <string>  
#include <iostream>

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

void saveLoad::encryptPowerup(int type, int shoes, int crowdcontrol, int cheapfood, 
                              int cheaprent, int richcustomers, int thriftycustomers)
{
    switch (type)
    {
    case 0: //encrypt
    {
        encryptedPowerup = (shoes * 2) * (crowdcontrol * 3) * (cheapfood * 5) *
                           (cheaprent * 7) * (richcustomers * 11) * (thriftycustomers * 13);

    }
    case 1: //decrypt
    {
        while (encryptedPowerup != 1)
        {
            if (encryptedPowerup % 13 == 0)
            {
                pThriftycustomers++;
                encryptedPowerup / 17;
            }
            else if (encryptedPowerup % 11 == 0)
            {
                pRichcustomers++;
                encryptedPowerup / 13;
            }
            else if (encryptedPowerup % 7 == 0)
            {
                pCheaprent++;
                encryptedPowerup / 11;
            }
            else if (encryptedPowerup % 5 == 0)
            {
                pCheapfood++;
                encryptedPowerup / 5;
            }
            else if (encryptedPowerup % 3 == 0)
            {
                pCrowdcontrol++;
                encryptedPowerup / 3;
            }
            else if (encryptedPowerup % 2 == 0)
            {
                pShoes++;
                encryptedPowerup / 2;
            }
        }
    }
    }
}

void saveLoad::loadSave(string save) //DOES NOT WORK, HOW THE FUCK DO YOU CALL A .TXT 
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

void saveLoad::saveSave(string save) //DOES NOT WORK, HOW THE FUCK DO YOU CALL A .TXT 
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
