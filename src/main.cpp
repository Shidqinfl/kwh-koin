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
#include "relay.h"
logger logg;
pzem pm;
WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_TOKEN,client);
coin coins;
relay relays;
String chat_id;
// wifi

void initWifi(){
  WiFi.setHostname("KWH Koin");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_Public, PASS_Public);
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // lcdPrint();
  }
  Serial.println("");
  Serial.println("[INFO] WiFi connected"); 
}
//telegram handler
bool relaystate=false;
void handleNewMessages(int numNewMessages){
  Serial.println("[HANDLER] handleNewMessages");
  Serial.println("[HANDLER] "+String(numNewMessages));
  String from_name ;

  sprintf(updata, "---");
  for (int i = 0; i < numNewMessages; i++)
  {
    chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

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
      relays.RelayOFF(4);
      relaystate = false;
    }
    if (text == "/forceStart" || text == "/forceStart@_")
    {
      bot.sendMessage(chat_id, "starting to ON relay", "");
      //function cut off
      relays.RelayON(4);
      relaystate = true;
    }

    if (text == "/options"|| text == "/options@_")
    {
      // // String keyboardJson = "[[\"/ph\", \"/waterquality\",\"/waterlevel\"],[\"/data\"]]";
      // bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }

    if (text == "/start" || text == "/start@")
    {
      String welcome = "Welcome to kwhcoingobang_Bot, " + from_name + ".\n";
      welcome += "Please Input these commands .\n\n";
      welcome += "/data: to get all parameters\n";
      // welcome += "/ph : to get pH data\n";
      // welcome += "/waterquality : to get water quality\n";
      // welcome += "/waterlevel : to get water level\n";
      // welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome.c_str(), "Markdown");
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

void i2cscan(){
  byte error, address;
  int Devices;
  Serial.println("Scanning...");
  Devices = 0;
  for(address = 1; address < 127; address++ )
  {

  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  if (error == 0)
  {
  Serial.print("I2C device found at address 0x");
  if (address<16)
  Serial.print("0");
  Serial.print(address,HEX);
  Serial.println("  !");
  Devices++;
  }
  else if (error==4)
  {
  Serial.print("Unknown error at address 0x");
  if (address<16)
  Serial.print("0");
  Serial.println(address,HEX);
  }
  }
  if (Devices == 0)
  Serial.println("No I2C devices found\n");
  else
  Serial.println("done\n");
  delay(5000);          

}
void initLCD(){
  lcd.init();              
  lcd.backlight();
  lcd.setContrast(100);
}
void lcdPrint(uint8_t col, uint8_t row, String data){// col 1 - 16 | row 0 - 1 | data string
    lcd.setCursor(col, row);
    lcd.print(data);
    // lcd.clear();
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
  logg.print("trace", "relay initialize");
  relays.init();
  for (size_t i = 1; i <=4; i++){
    relays.RelayOFF(i);
  }
  logg.print("trace", "PM initialize");
  pm.init(PMid);
  logg.print("debug", "PM ID: "+  String(PMid));
  logg.print("trace", "Coin Selector initialize");
  coins.init();
  logg.print("trace", "LCD 16x2 initialize");
  initLCD(); 
  lcdPrint(3,0,"Starting");
  logg.print("trace", "WiFI");
  initWifi();
}
int thresholdTime = 10; //menit
int thresholdEnergy = 1; //kwh


unsigned long prevMillis = 0;
void main1(){
  unsigned long currMillis= millis();
  botHandlerMessage();
  lcdPrint(2,0,"Insert Coin");
  lcdPrint(4,1,"Rp.1000");
  unsigned long currMillis = millis();
  int coinImpulse = coins.readImpulse();
  unsigned long sectick = 0;
  while (coinImpulse != 0 || relaystate ==true){ 
    botHandlerMessage();
    Serial.println("=====================");
    lcd.clear();
    pm.singlePhase(PMid, 0);
    float voltage =  pm.Cur_voltage();
    float current = pm.Cur_current();
    float power = pm.Cur_power();
    float energy = pm.Cur_energy();
    lcdPrint(0, 0, "Voltage");
    lcdPrint(8, 0, String(voltage));
    delay(2000);
    lcdPrint(0, 0, "Current");
    lcdPrint(8, 0, String(current));
    delay(2000);
    lcdPrint(0, 0, "Power  ");
    lcdPrint(8, 0, String(power));
    delay(2000);
    lcdPrint(0, 0, "Energy  ");
    lcdPrint(8, 0, String(energy));
    delay(2000);
    lcd.clear();  
    if(currMillis - prevMillis >= 120000){
      uplink("Voltage: "+ String(voltage) + " v \n Current:"+ String(current) + " a \n Power: " +String(power) + " w \n Energy: "+ String(energy)+ " w");
    }
    if(currMillis - prevMillis >= 1000){
      sectick+1;
    }
    thresholdEnergy = thresholdEnergy * coinImpulse;
    thresholdTime = thresholdTime * coinImpulse;
    for (size_t i = 1; i <=4; i++)
    {
      relays.RelayON(i);
    }
    
    if(energy == thresholdEnergy || relaystate == false || sectick == 600000) {
      for (size_t i = 1; i <=4; i++)
      {
        relays.RelayOFF(i);
        coins.resetImpulse();
        break;
        sectick =0;
      }
    }
    prevMillis = currMillis;
  }
}



void test(){
  lcdPrint(0,0,"start");
  pm.singlePhase(PMid, 0);
  float voltage =  pm.singlePhase(PMid, 1);
  float current = pm.singlePhase(PMid, 2);
  float power = pm.singlePhase(PMid, 3);
  float energy = pm.singlePhase(PMid, 4);
  lcdPrint(0, 0, "Voltage");
  lcdPrint(8, 0, String(voltage));
  delay(2000);
  lcdPrint(0, 0, "Current");
  lcdPrint(8, 0, String(current));
  delay(2000);
  lcdPrint(0, 0, "Power  ");
  lcdPrint(8, 0, String(power));
  delay(2000);
  lcdPrint(0, 0, "Energy  ");
  lcdPrint(8, 0, String(energy));
  delay(2000);
  lcd.clear();
  coins.readImpulse();
}

void loop() {
  // i2cscan();
  // while (1)
  // {
  //   test();
  // }
  main1();
}
