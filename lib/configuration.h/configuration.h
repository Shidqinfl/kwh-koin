#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Arduino.h"


#define SSID_Public "ga ada"
#define PASS_Public "Namakamu"
const static char* TELEGRAM_TOKEN = "6974967047:AAGAQQPMvVLHg1crul3th3lpsfqRky-nczc";
const static char* forceClientID = "735707164";

const unsigned long BOT_MTBS = 1000;
static unsigned long bot_lasttime; 
static char updata[200] = "";

static int lcdColumns = 16;
static int lcdRows = 2;
#define reg_LCD 0x27
#define PMid 1
static float KwhTreshold = 30.0;


#endif 