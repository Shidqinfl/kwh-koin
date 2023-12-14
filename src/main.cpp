/*
  assume 
    1 coin = 30kwh
    1 coin = 1 hour
    

*/

#include <Arduino.h>
#include <WiFi.h>
#include "configuration.h"
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "pzem.h"
#include "logger.h"
#include "coin.h"

String chat_id;
logger logg;
pzem pm;
WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_TOKEN,client);
coin coins;
// wifi
void initWifi(){
  WiFi.setHostname("KWH Koin");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_Public, PASS_Public);
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("[INFO] WiFi connected"); 
}
//telegram handler

void handleNewMessages(int numNewMessages){
  Serial.println("[HANDLER] handleNewMessages");
  Serial.println("[HANDLER] "+String(numNewMessages));
  String from_name ;

  for (int i = 0; i < numNewMessages; i++)
  {
    chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
  sprintf(updata, "---");

    from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/data" || text == "/data@_")
    {
      bot.sendMessage(chat_id, updata, "");
    }
    
    if (text == "/forcestop" || text == "/forcestop@_")
    {
      bot.sendMessage(chat_id, "starting to cut off user", "");
      //function cut off
    }

    if (text == "/options"|| text == "/options@_")
    {
      // // String keyboardJson = "[[\"/ph\", \"/waterquality\",\"/waterlevel\"],[\"/data\"]]";
      // bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }

    if (text == "/start" || text == "/start@TandonMonitor_bot")
    {
      String welcome = "Welcome to TandonMonitor_Bot, " + from_name + ".\n";
      welcome += "Please Input these commands .\n\n";
      welcome += "/data: to get all parameters\n";
      // welcome += "/ph : to get pH data\n";
      // welcome += "/waterquality : to get water quality\n";
      // welcome += "/waterlevel : to get water level\n";
      // welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
  Serial.println("[HANDLER] " + from_name);
}
void botHandlerMessage(){
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("[HANDLER] got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
void uplink(String payload){
  bot.sendMessage(forceClientID, payload,"");
  //payload
}

//LCD

LiquidCrystal_I2C lcd(reg_LCD, lcdColumns, lcdRows);  

void initLCD(){
  lcd.init();              
  lcd.backlight();
}
void lcdPrint(uint8_t col, uint8_t row, String data){// col 1 - 16 | row 0 - 1 | data string
    lcd.setCursor(col, row);
    lcd.print(data);
}

void lcdPrintScroll(int row, String message, int delayTime, int lcdColumns){
    for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup() {
  logg.init();
  logg.print("trace", "start");
  pm.init(PMid);
  coins.init();
  initLCD(); 
}
void main1(){
  int thresholdTime = 30; //menit
  int thresholdEnergy = 10; //kwh

  float voltage =  pm.Cur_voltage();
  float current = pm.Cur_current();
  float power = pm.Cur_power();
  float energy = pm.Cur_energy();

  int coinImpulse = coins.readImpulse();
  if (coinImpulse!=0){
    thresholdEnergy = thresholdEnergy * coinImpulse;
    thresholdTime = thresholdTime * coinImpulse;
    //relay_on(duration, limitenergy);
    if(energy == thresholdEnergy) {
      //relay_off;
    }

  }
}
void loop() {
  lcdPrint(0,0,"start");
  pm.singlePhase(PMid, 0);
  float voltage =  pm.singlePhase(PMid, 1);
  float current = pm.singlePhase(PMid, 2);
  float power = pm.singlePhase(PMid, 3);
  float energy = pm.singlePhase(PMid, 4);

  coins.readImpulse();
}
