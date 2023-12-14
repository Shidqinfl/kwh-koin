
#include "ntp.h"

void ntp:: NtpInit(){
    const char* ntpServer = "0.id.pool.ntp.org";
    const long gmtOffset_sec = 25200;//utc+7 //3600*7
    const int daylightOffset_sec = 0;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
String ntp:: GetTimestamp(){
    struct tm timeinfo;
    String timestamp;
    if(!getLocalTime(&timeinfo)){
        logg.print("error","FAILED getNTP");
        timestamp="1970-01-01T00:00:00+07:00";
    }else{
        char datetime[30];
        strftime(datetime, 30, "%FT%T+07:00", &timeinfo);
        timestamp = String(datetime);
    }
    return timestamp;
}
uint8_t ntp:: GetHour(){
    struct tm timeinfo;
    uint8_t result;
    if(!getLocalTime(&timeinfo)){
        logg.print("error","FAILED get hour");
        result= 0;
    }else{
        char times[30];
        strftime(times, 30, "%H", &timeinfo);
        result = String(times).toInt();
    }
    return result;
}
uint8_t ntp:: Getminute(){
    struct tm timeinfo;
    uint8_t result;
    if(!getLocalTime(&timeinfo)){
        logg.print("error","FAILED get minute");
        result= 0;
    }else{
        char times[30];
        strftime(times, 30, "%M", &timeinfo);
        result = String(times).toInt();
    }
    return result;

}
uint8_t ntp:: GetSecond(){
    struct tm timeinfo;
    uint8_t result;
    if(!getLocalTime(&timeinfo)){
        logg.print("error","FAILED get minute");
        result= 0;
    }else{
        char times[30];
        strftime(times, 30, "%S", &timeinfo);
        result = String(times).toInt();
    }
    return result;
}
uint8_t ntp :: TresholdTime(uint8_t pulse){
    return GetHour() + pulse;
}