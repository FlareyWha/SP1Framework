#pragma once
#include "Entity.h"
#include "game.h"

class Shelf :
    public Entity
{
private:
    const int capacity;//Capacity of the shelf
    int amount;// Current amount on shelf
    int itemtag;//Type of item the shelf will have
    WORD colour;

public:
    Shelf();
    ~Shelf();
    int getCapacity();
    int getAmount();
    void setAmount(int amount);
    int getItemtag();
    void setItemtag(int itemtag);

    void setShelf(WORD c);
    WORD returnShelfColour();
};

