#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Arduino.h"


#define SSID_Public "OPPO F7"
#define PASS_Public "wawaydeng"
const static char* TELEGRAM_TOKEN = "6641457820:AAFhg7hX9MaEQjVSc-ksd4o7ZNkZtvxH6UQ";
const static char* forceClientID = "875203901";

const unsigned long BOT_MTBS = 1000;
static unsigned long bot_lasttime; 
static char updata[200] = "";

static int lcdColumns = 16;
static int lcdRows = 2;
#define reg_LCD 0x27
#define PMid 1
static float KwhTreshold = 30.0;


#endif 