#pragma once
#include "Entity.h"
class Son :
    public Entity
{
private:
    bool bSick;
    bool btoggleFood; //Used to check if player has decided to feed his son
    int NoOfDaysUnfed;

public:
    Son();
    ~Son();

    void isSick(); //Son falls sick, bSick = true
    void Recovers(); //son recovers, bSick = false
    bool getStatus(); //return bSick
    void isFed(); // Home menu food option is toggled true
    void isnotFed(); // Home menu food option is toggled false

    void increaseNODUnfed();
    void decreaseNODUnfed();
    void resetNODUnfed();
    int getNODUnfed();

    void ChancesOfFallingSick(Son s); 
};

