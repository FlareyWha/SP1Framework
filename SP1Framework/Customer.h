#pragma once
#include "Entity.h"
#include "Map.h"
#include "Windows.h"
#include "Position.h"

class Customer :
    public Entity
{
private:
    int passcheck;
    int itemToBuy;//to determine which item the customer is going to buy
    int quantity;//to determine how many of the item the customer is going to buy
    int avoiding;
    bool bSatisfied;
    bool yLock;
    bool travelling;
    Position endPoint;
    Position prevPos;
    int CustomerDirection;

public:
    Customer();
    ~Customer();
    void moveToShelfContainingItem(int itemToBuy);
    bool entranceWaiting(void);
    bool shelfWaiting(void);
    void isSatisfied();
    void unSatisfied();


    bool getState();
    bool getTravelling();
    int getQuantity();
    int getItemToBuy();
    int getX();
    int getY();
    int getEndPointX();
    int getEndPointY();
    Position getEndPoint();
    void setYLock(bool set);
    void setEndPoint(int x, int y);
    void setQuantity(int quantity);
    void setItemToBuy(int passcheck);
    void setAvoiding(int number);
    void setTravelling(int travelling);
    void printOutCustomer(bool spawned, Console& console, Position pos, Map& map, int q, WORD s);
    int moveCustomer(Map& map, int framesPassed, int timer);
    void customerCollision(Map& map, bool& travelling);
};

