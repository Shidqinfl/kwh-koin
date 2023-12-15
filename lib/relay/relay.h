#ifndef RELAY_H
#define RELAY_H
#include "Arduino.h"



class relay
{
private:
    /* data */
public:
    void init();
    int RelayON(int chan);
    int RelayOFF(int chan);
};
#endif