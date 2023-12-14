#ifndef PZEM_H
#define PZEM_H

#include <Arduino.h>
#include "configuration.h"
#include "logger.h"

class pzem{
    logger logg;
    private:
    public:
    void init(uint8_t id);
    String threePhase();
    float singlePhase(uint8_t pm_id, uint8_t type);
    float Cur_voltage();
    float Cur_current();
    float Cur_power();  
    float Cur_energy();  
    void reset_energy();
};

#endif 