#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Arduino.h"


#define SSID_Public "Aziz"
#define PASS_Public "12345678"
const static char* TELEGRAM_TOKEN = "6887038846:AAFtCG2Nd8nnhEqCvswKLiLqnsGSQ4D-3PA";
const static char* forceClientID = "1922118805";

const unsigned long BOT_MTBS = 1000;
static unsigned long bot_lasttime; 
static char updata[200] = "";

static int lcdColumns = 16;
static int lcdRows = 2;
#define reg_LCD 0x27
#define PMid 1
static float KwhTreshold = 30.0;


#endif 