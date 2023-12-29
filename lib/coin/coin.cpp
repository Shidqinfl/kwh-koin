#include "coin.h"
#include "EEPROM.h"
int impulse = 0;
uint32_t resImpulse;
int TotalAmmount = 0;
int pincoin = 13;
// void IRAM_ATTR isr(){
//     impulse=impulse+1;
//     resImpulse=0;
// }
// volatile byte stateBackup = LOW;
void cointrig(){
    impulse=impulse+1;
    resImpulse=0;
    Serial.print("| PULSE HIGH = ");  Serial.print(impulse);  Serial.println(" |");
     //Serial.println(digitalRead(pincoin)); 
}
void coin::init(){
    // pinMode(pincoin, INPUT);
    attachInterrupt(digitalPinToInterrupt(13),cointrig, FALLING);
    EEPROM.begin(255);
    EEPROM.put(1, TotalAmmount);
}

int coin::readImpulse(){
    // Serial.print("pulse: ");  Serial.println(digitalRead(pincoin)); 
    resImpulse=resImpulse+1;
    // Serial.print("1 | resImpulse: ");  Serial.println(resImpulse); 
    // Serial.print("2 | Dgital 13 : ");  Serial.println(digitalRead(pincoin)); 
    if(resImpulse >=30 && impulse==1){
        TotalAmmount=TotalAmmount+1;
        impulse = 0;
        EEPROM.put(1, TotalAmmount);
    }
    EEPROM.commit();
    int rst =  EEPROM.get(1,TotalAmmount);
    // Serial.print("3 | EPROM     : ");  Serial.println(rst); 
    return rst;
}
void coin::resetImpulse(){
    TotalAmmount = 0;
    EEPROM.put(1, TotalAmmount);
}
