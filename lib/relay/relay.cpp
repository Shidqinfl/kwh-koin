#include "relay.h"
#define PIN_RELAY_1  27
#define PIN_RELAY_2  26
#define PIN_RELAY_3  25
#define PIN_RELAY_4  33

//func init pin relay
//func on(int)
//func off(int)
void relay::init(){
    pinMode(PIN_RELAY_1, OUTPUT);
    pinMode(PIN_RELAY_2, OUTPUT);
    pinMode(PIN_RELAY_3, OUTPUT);
    pinMode(PIN_RELAY_4, OUTPUT);
}
int relay::RelayOFF(int chan){ //1 -4
    int num;
    switch (chan)
    {
    case 1:
        digitalWrite(PIN_RELAY_1, HIGH);
        num = 0x1;
        break;
    case 2:
        digitalWrite(PIN_RELAY_2, HIGH);
        num = 0x2;
        break;
    case 3:
        digitalWrite(PIN_RELAY_3, HIGH);
        num = 0x3;
        break;
    case 4:
        digitalWrite(PIN_RELAY_4, HIGH);
        num = 0x4;
        break;
    }
 return num;
}

int relay::RelayON(int chan){
    int num;
    switch (chan)
    {
    case 1:
        digitalWrite(PIN_RELAY_1, LOW);
        num = 0x1;
        break;
    case 2:
        digitalWrite(PIN_RELAY_2, LOW);
        num = 0x2;
        break;
    case 3:
        digitalWrite(PIN_RELAY_3, LOW);
        num = 0x3;
        break;
    case 4:
        digitalWrite(PIN_RELAY_4, LOW);
        num = 0x4;
        break;
    }
 return num;
}