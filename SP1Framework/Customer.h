#pragma once
#include "Entity.h"
#include "Map.h"
#include "Windows.h"
#include "Position.h"
#include "Player.h"

class Customer :
    public Entity
{
private:
    double timer;
    int passcheck;
    int itemToBuy;//to determine which item the customer is going to buy
    int quantity;//to determine how many of the item the customer is going to buy
    int avoiding;
    bool bSatisfied;
    bool yLock;
    bool travelling;
    bool spawned;
    bool movingBack;
    bool timerSet;
    Position endPoint;
    Position prevPos;
    int CustomerDirection;

    bool colliding;
    

public:
    Customer();
    Customer(Player p);
    ~Customer();
    void moveToShelfContainingItem(int itemToBuy);
    bool entranceWaiting(void);
    bool shelfWaiting(void);
    void isSatisfied();
    void unSatisfied();

    bool getState();
    int getQuantity();
    int getItemToBuy();
    int getX();
    int getY();
    int getEndPointX();
    int getEndPointY();
    Position getEndPoint();
    bool getTravelling();
    bool getMovingBack();
    bool getTimerSet();
    double getTimer();
    bool getColliding();//delete
    Position getPrevPos();//delete
    int getAvoiding(); //delete
    void setYLock(bool set);
    void setEndPoint(int x, int y);
    void setQuantity(int quantity);
    void setItemToBuy(int passcheck);
    void setAvoiding(int number);
    void setTravelling(bool travelling);
    void setSpawned(bool spawned);
    void setMovingBack(bool movingBack);
    void setTimer(double timer);
    void addTimer(double dt);
    void setTimerSet(bool timerSet);
    void printOutCustomer(Console& console, Map& map, int q, WORD s);
    int moveCustomer(Map& map, int framesPassed, int timer);
    void customerCollision(Map& map);
};

