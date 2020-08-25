#include "Shelf.h"

Shelf::Shelf() : capacity{ 9 } , amount{0} , itemtag{ ' ' }, colour{ 0x00 }
{
}

Shelf::~Shelf()
{
}

int Shelf::getCapacity()
{
    return capacity;
}

int Shelf::getAmount()
{
    return amount;
}

void Shelf::setAmount(int amount)
{
    this->amount = amount;
}

int Shelf::getItemtag()
{
    return itemtag;
}

void Shelf::setItemtag(int itemtag)
{
    this->itemtag = itemtag;
}

void Shelf::decreaseItem(int i)
{
 
    amount = amount - i;
    
}

void Shelf::setShelf(WORD c) {
    colour = c;
}

WORD Shelf::returnShelfColour() {
    return colour;
}

void Shelf::increaseItem(int i) {
    amount = amount + i;
    if (amount > capacity)
    {
        amount = capacity;
    }
}

