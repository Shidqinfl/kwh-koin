#ifndef COIN_H
#define COIN_H
#include "Arduino.h"

class coin
{
private:
    /* data */
public:
    // int impulse;
    // int resImpulse;
    // int TotalAmmount = 0;
    void init();
    // void incommingImpulse();
    int readImpulse();
    void resetImpulse();
    
};


#endif