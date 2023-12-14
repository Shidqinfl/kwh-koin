#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class logger{
    private:
    public:
        void init();
        void print(String level, String msg);
};

#endif 