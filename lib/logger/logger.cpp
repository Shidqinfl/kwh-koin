#include "logger.h"

#include <Arduino.h>

void logger::init(){
    Serial.begin(115200,SERIAL_8N1);
    delay(1000);
}

void logger::print(String level, String msg){
    if (level=="panic"){
        Serial.printf("[ PANIC ] %s\n",msg.c_str());
    }else if (level=="error"){
        Serial.printf("[ ERROR ] %s\n",msg.c_str());
    }else if (level=="warning"){
        Serial.printf("[WARNING] %s\n",msg.c_str());
    }else if (level=="info"){
        Serial.printf("[ INFO  ] %s\n",msg.c_str());
    }else if (level=="debug"){
        Serial.printf("[ DEBUG ] %s\n",msg.c_str());
    }else if (level=="trace"){
        Serial.printf("[ TRACE ] %s\n",msg.c_str());
    }else if (level=="newline"){
        Serial.println();
    }
}