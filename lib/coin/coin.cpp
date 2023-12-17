#include "coin.h"
#include "EEPROM.h"
int impulse = 0;
int resImpulse=0;
int TotalAmmount = 0;
int pincoin = 13;
void IRAM_ATTR isr(){
    impulse=impulse+1;
    resImpulse=0;
}
void coin::init(){
    attachInterrupt(pincoin,isr, FALLING);
    EEPROM.get(0, TotalAmmount);
}
int coin::readImpulse(){
    resImpulse=resImpulse+1;
    if(resImpulse >=30 && impulse==1){
        TotalAmmount=TotalAmmount+1;
        impulse = 0;
        EEPROM.put(0, TotalAmmount);
    }
    EEPROM.commit();
    return EEPROM.get(0,TotalAmmount);
}
void coin::resetImpulse(){
    TotalAmmount = 0;
    EEPROM.put(0, TotalAmmount);
}
