#pragma once
#include "Entity.h"
class Son :
    public Entity
{
private:
    bool bSick;
    bool btoggleFood; //Used to check if player has decided to feed his son
    bool bHospitalised;
    int NoOfDaysUnfed;
    int NoOfDaysSick;

public:
    Son();
    ~Son();

    void isSick(); //Son falls sick, bSick = true
    void Recovers(); //son recovers, bSick = false
    bool getStatus(); //return bSick
    void isHosp();
    bool getHospState();
    void isFed(); // Home menu food option is toggled true
    bool getStatusFed(); // return btoggleFood

    void increaseNODUnfed();
    void decreaseNODUnfed();
    void resetNODUnfed();
    int getNODUnfed();
    void increaseNODSick();
    void resetNODSick();
    int getNODSick();

    void ChancesOfFallingSick(int daysUnfed); 
};

