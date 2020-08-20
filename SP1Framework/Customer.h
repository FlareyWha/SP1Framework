#pragma once
#include "Entity.h"
#include "Map.h"
#include "Windows.h"
#include "Position.h"
class Customer :
    public Entity
{
private:
    int itemToBuy;//to determine which item the customer is going to buy
    int quantity;//to determine how many of the item the customer is going to buy
    bool bSatisfied;
public:
    Customer();
    ~Customer();
    void moveToShelfContainingItem(int itemToBuy);
    bool entranceWaiting(void);
    bool shelfWaiting(void);
    void isSatisfied();
    void unSatisfied();
    bool getState();
    int getQuantity();
    int getItemToBuy();
    void printOutCustomer(bool spawned, Console& console, Position pos, Map& map);
};

