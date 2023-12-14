#ifndef NTP_H
#define NTP_H
#include "Arduino.h"
#include "logger.h"
class ntp{
    logger logg;
    private:
    public:
        void NtpInit();
        String GetTimestamp();
        uint8_t GetHour();
        uint8_t Getminute();
        uint8_t GetSecond();
        uint8_t TresholdTime(uint8_t pulse);
};


#endif 
