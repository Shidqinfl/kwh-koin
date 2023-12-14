#include "pzem.h"

#include <Arduino.h>
#include <PZEM004Tv30.h>

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif
#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif
#define QTY_PZEMS 1 //quantity of pzem
PZEM004Tv30 Dpzem;

void pzem::init(uint8_t id){
    Dpzem = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x0 + id);//start from pm id 10
    delay(1000);
}


float pzem::singlePhase(uint8_t pm_id, uint8_t type){
    char pm[200];
    float data;
    uint8_t id = Dpzem.getAddress();
    if (id==pm_id){
        float voltage = Dpzem.voltage();
        float current = Dpzem.current();
        float power = Dpzem.power();
        float energy = Dpzem.energy();
        if( isnan(current)|| isnan(power)|| isnan(energy) ){
            logg.print("error", "failed get pmID:"+String(id));
            sprintf(pm, "{\"id\":%d,\"voltage\":%s,\"current\":%s,\"power\":%s,\"energy\":%s}", pm_id,"0.0","0.0","-1.0","0.0");   
        }else{
            sprintf(pm, "{\"id\":%d,\"voltage\":%s,\"current\":%s,\"power\":%s,\"energy\":%s}", pm_id, String(voltage).c_str(),String(current).c_str(),String(power).c_str(),String(energy).c_str()); 
        }   

        switch (type){
        case 0:
            logg.print("debug", String(pm));
            break;
        case 1:
            data = voltage;
            break;
        case 2:
            /* code */
            data = current;
            break;
        case 3:
            /* code */
            data = power;
            break;
        case 5:
            /* code */
            data = energy;
        break;
        }    
    }
   
    // String dataStr=String(pm);
    memset(pm, 0, sizeof(pm));

    return data;
}

float pzem :: Cur_voltage(){
    return pzem::singlePhase(PMid,1);
}
float pzem :: Cur_current(){
    return pzem::singlePhase(PMid,2);
}
float pzem :: Cur_power(){
    return pzem::singlePhase(PMid,3);
}
float pzem :: Cur_energy(){
    return pzem::singlePhase(PMid,4);
}
void pzem::reset_energy(){
    Dpzem.resetEnergy();
}

