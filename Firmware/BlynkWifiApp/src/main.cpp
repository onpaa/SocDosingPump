#define BLYNK_PRINT Serial
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

#include <LearningKit.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <EEPROM.h>

char auth[] = "b5ac866ca280448cae7a375658aa923d";


////char ssid[] = "Ondřej - iPhone";
//char pass[] = "69696969";
char ssid[] = "WL520GC";
char pass[] = "klemont1";
//char ssid[] = "SPAVOS_NET";
//char pass[] = "m0gu145.";

BlynkTimer timer;
WidgetRTC rtc;

int pumpSettingChoice = 0;
int pumpSettingChoice1 = 0;
long stopPump1 = 0;
long stopPump2 = 0;
static const uint8_t pump1dir = 16;
static const uint8_t pump2dir = 0;
static const uint8_t motor1pin = 17;
static const uint8_t motor2pin = 4;

#include "data.hpp"

static const int numberOfPump = 2;
Pump pumps[numberOfPump];

#include "blynk_functions.hpp"

void pumpAllValue() 
{
  if (pumps[0].pumpStatus==1)
  {
    String rowStatus = String("Status                                  ") + "ON";
    Blynk.virtualWrite(V3, rowStatus);
  }
  else if (pumps[0].pumpStatus==0)
  {
    String rowStatus = String("Status                                  ") + "OFF";
    Blynk.virtualWrite(V3, rowStatus);
  }

  if(pumps[0].modeSelect == 0)
  {
    if(pumps[0].hourDailyMode<10 && pumps[0].minuteDailyMode<10 )
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[0].hourDailyMode + ":" + "0"+ pumps[0].minuteDailyMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else if (pumps[0].hourDailyMode<10)
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[0].hourDailyMode + ":" +  pumps[0].minuteDailyMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else if(pumps[0].minuteDailyMode<10 )
    {
      String rowTime = String("Time                                   ") + pumps[0].hourDailyMode + ":" + "0"+ pumps[0].minuteDailyMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else
    {
      String rowTime = String("Time                                    ") + pumps[0].hourDailyMode + ":" + pumps[0].minuteDailyMode;
      Blynk.virtualWrite(V4, rowTime);
    }

    if (pumps[0].dlDailyMode==0)
    {
      String rowAmount = String("Amount                               ") + pumps[0].mlDailyMode + " ml";
      Blynk.virtualWrite(V5, rowAmount);
    }
    else
    {
      String rowAmount = String("Amount              ") + pumps[0].dlDailyMode + " dl   " + pumps[0].mlDailyMode + " ml       ";
      Blynk.virtualWrite(V5, rowAmount);
    } 
  }

  else if(pumps[0].modeSelect == 1)
  {
    if(pumps[0].hourPeriodMode<10 && pumps[0].minutePeriodMode<10 )
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[0].hourPeriodMode + ":" + "0"+ pumps[0].minutePeriodMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else if (pumps[0].hourPeriodMode<10)
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[0].hourPeriodMode + ":" +  pumps[0].minutePeriodMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else if(pumps[0].minutePeriodMode<10 )
    {
      String rowTime = String("Time                                   ") + pumps[0].hourPeriodMode + ":" + "0"+ pumps[0].minutePeriodMode;
      Blynk.virtualWrite(V4, rowTime);
    }
    else
    {
      String rowTime = String("Time                                    ") + pumps[0].hourPeriodMode + ":" + pumps[0].minutePeriodMode;
      Blynk.virtualWrite(V4, rowTime);
    }

    if (pumps[0].dlPeriodMode==0)
    {
      String rowAmount = String("Amount                               ") + pumps[0].mlPeriodMode + " ml";
      Blynk.virtualWrite(V5, rowAmount);
    }
    else
    {
      String rowAmount = String("Amount              ") + pumps[0].dlPeriodMode + " dl   " + pumps[0].mlPeriodMode + " ml       ";
      Blynk.virtualWrite(V5, rowAmount);
    }
  }
}
void pumpAllValue2() 
{
  if (pumps[1].pumpStatus==1)
  {
    String rowStatus = String("Status                                  ") + "ON";
    Blynk.virtualWrite(V10, rowStatus);
  }
  else if (pumps[1].pumpStatus==0)
  {
    String rowStatus = String("Status                                  ") + "OFF";
    Blynk.virtualWrite(V10, rowStatus);
  }

  if(pumps[1].modeSelect == 0)
  {
    if(pumps[1].hourDailyMode<10 && pumps[0].minuteDailyMode<10 )
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[1].hourDailyMode + ":" + "0"+ pumps[1].minuteDailyMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else if (pumps[1].hourDailyMode<10)
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[1].hourDailyMode + ":" +  pumps[1].minuteDailyMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else if(pumps[1].minuteDailyMode<10 )
    {
      String rowTime = String("Time                                   ") + pumps[1].hourDailyMode + ":" + "0"+ pumps[1].minuteDailyMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else
    {
      String rowTime = String("Time                                    ") + pumps[1].hourDailyMode + ":" + pumps[1].minuteDailyMode;
      Blynk.virtualWrite(V12, rowTime);
    }

    if (pumps[1].dlDailyMode==0)
    {
      String rowAmount = String("Amount                               ") + pumps[1].mlDailyMode + " ml";
      Blynk.virtualWrite(V11, rowAmount);
    }
    else
    {
      String rowAmount = String("Amount              ") + pumps[1].dlDailyMode + " dl   " + pumps[1].mlDailyMode + " ml       ";
      Blynk.virtualWrite(V11, rowAmount);
    } 
  }

  else if(pumps[1].modeSelect == 1)
  {
    if(pumps[1].hourPeriodMode<10 && pumps[1].minutePeriodMode<10 )
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[1].hourPeriodMode + ":" + "0"+ pumps[1].minutePeriodMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else if (pumps[1].hourPeriodMode<10)
    {
      String rowTime = String("Time                                   ")+ "0" + pumps[1].hourPeriodMode + ":" +  pumps[1].minutePeriodMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else if(pumps[1].minutePeriodMode<10 )
    {
      String rowTime = String("Time                                   ") + pumps[1].hourPeriodMode + ":" + "0"+ pumps[1].minutePeriodMode;
      Blynk.virtualWrite(V12, rowTime);
    }
    else
    {
      String rowTime = String("Time                                    ") + pumps[1].hourPeriodMode + ":" + pumps[1].minutePeriodMode;
      Blynk.virtualWrite(V12, rowTime);
    }

    if (pumps[1].dlPeriodMode==0)
    {
      String rowAmount = String("Amount                               ") + pumps[1].mlPeriodMode + " ml";
      Blynk.virtualWrite(V11, rowAmount);
    }
    else
    {
      String rowAmount = String("Amount              ") + pumps[1].dlPeriodMode + " dl   " + pumps[1].mlPeriodMode + " ml       ";
      Blynk.virtualWrite(V11, rowAmount);
    }
  }
}

void getTime()
{
    String currentTime = String(hour()) + ":" + minute() + ":" + second();
    Blynk.virtualWrite(V22, currentTime);
}

void pumpStop()
{
  long timeInSecond = hour()*3600+minute()*60+second();
  if(timeInSecond == stopPump1)
  {
    digitalWrite(motor1pin, LOW);
    stopPump1 = 0;
  }
  if(timeInSecond == stopPump2)
  {
    digitalWrite(motor2pin, LOW);
    stopPump2 = 0;
  }
}
void pumpStart(long dl, long ml, long HOUR, int MINUTE, uint16_t SECOND, uint8_t pumpNumber)
{
  Serial.println("Ve smycce pumpStart");
  long timeInSecond = 0;
  uint16_t amount = ((dl*100)+ml); 
  uint16_t constTime = 1000;

  HOUR *= 3600;
  MINUTE *= 60;
  

  if(pumps[0].modeSelect == 1)
  {
    timeInSecond = HOUR+MINUTE+SECOND+pumps[0].mutliple*3600;
    SECOND = timeInSecond % 60;
    timeInSecond /= 60;
    MINUTE = timeInSecond % 60;
    timeInSecond /= 60;
    HOUR = timeInSecond % 24;
  }
  else if(pumps[1].modeSelect == 1)
  {
    timeInSecond = HOUR+MINUTE+SECOND+pumps[1].mutliple*3600;
    SECOND = timeInSecond % 60;
    timeInSecond /= 60;
    MINUTE = timeInSecond % 60;
    timeInSecond /= 60;
    HOUR = timeInSecond % 24;
  }
  else if(pumps[0].modeSelect == 0) {}
  else if(pumps[1].modeSelect == 0) {}
  else {}
  
  // EEPROM write
  if(pumps[0].modeSelect == 1)
  {
    pumps[0].hourPeriodMode = HOUR;
    pumps[0].minutePeriodMode = MINUTE;
    pumps[0].secondPeriodMode = SECOND;
    pumps[0].changed = true;
  }
  else if(pumps[1].modeSelect == 1)
  {
    pumps[1].hourPeriodMode = HOUR;
    pumps[1].minutePeriodMode = MINUTE;
    pumps[1].secondPeriodMode = SECOND;
    pumps[1].changed = true;
  }
  else if(pumps[0].modeSelect == 0)
  {
    pumps[0].hourDailyMode = HOUR;
    pumps[0].minuteDailyMode = MINUTE;
    pumps[0].secondDailyMode = SECOND;
    pumps[0].changed = true;
  }
  else if(pumps[1].modeSelect == 0)
  {
    pumps[1].hourDailyMode = HOUR;
    pumps[1].minuteDailyMode = MINUTE;
    pumps[1].secondDailyMode = SECOND;
    pumps[1].changed = true;
  }
  else{}
  

  if(pumpNumber == 1)
  {
    stopPump1 =  hour()*3600+minute()*60+second()+amount*constTime;
    digitalWrite(motor1pin, HIGH);
  }
  if(pumpNumber == 2)
  {
    stopPump2 = hour()*3600+minute()*60+second()+amount*constTime;
    digitalWrite(motor2pin, HIGH);
  }
}


void timeTesting()
{ 
  if(pumps[0].pumpStatus == 1 || pumps[1].pumpStatus == 1)
  {
    if(pumps[0].modeSelect == 1 || pumps[1].modeSelect == 1)
    {
      if(pumps[0].hourPeriodMode == hour() && pumps[0].minutePeriodMode == minute()	&& pumps[0].secondPeriodMode == second()) //pump1 period mode
      { 
        uint8_t pumpNumber = 1;
        pumpStart(pumps[0].dlPeriodMode,pumps[0].mlPeriodMode,pumps[0].hourPeriodMode,pumps[0].minutePeriodMode,pumps[0].secondPeriodMode,pumpNumber);
      }
      if(pumps[1].hourPeriodMode == hour() && pumps[1].minutePeriodMode == minute()	&& pumps[1].secondPeriodMode == second()) //pump2 period mode
      { 
        uint8_t pumpNumber = 2;
        pumpStart(pumps[1].dlPeriodMode,pumps[1].mlPeriodMode,pumps[1].hourPeriodMode,pumps[1].minutePeriodMode,pumps[1].secondPeriodMode,pumpNumber);
      }
    }
    else if (pumps[0].modeSelect == 0 || pumps[1].modeSelect == 0)
    {

    }
    else if(pumps[0].modeSelect != 0 && pumps[0].modeSelect != 1) { 
      Serial.println("Please select mode for pumps 1");
    }
    else if(pumps[1].modeSelect != 0 && pumps[1].modeSelect != 1) { 
      Serial.println("Please select mode for pumps 2");
    }
  }
}

void printActSetVal()
{
  Blynk.virtualWrite(V18, pumps[pumpSettingChoice].mlPeriodMode); //ml
  Blynk.virtualWrite(V17, pumps[pumpSettingChoice].dlPeriodMode); //dl
  Blynk.virtualWrite(V20, pumps[pumpSettingChoice].mutliple); //period
  if (pumpSettingChoice == 0)
  {
    Blynk.setProperty(V20, "color", "#5F7CD8");
    Blynk.setProperty(V9, "color",  "#5F7CD8");
    Blynk.setProperty(V19, "color", "#5F7CD8");
    Blynk.setProperty(V17, "color", "#5F7CD8");
    Blynk.setProperty(V18, "color", "#5F7CD8");
    Blynk.setProperty(V15, "color", "#5F7CD8");
    Blynk.setProperty(V16, "color", "#5F7CD8");
    Blynk.setProperty(V32, "color", "#5F7CD8");
  }
  else if (pumpSettingChoice == 1)
  {
    Blynk.setProperty(V20, "color", "#ED9D00");
    Blynk.setProperty(V9, "color",  "#ED9D00");
    Blynk.setProperty(V19, "color", "#ED9D00");
    Blynk.setProperty(V17, "color", "#ED9D00");
    Blynk.setProperty(V18, "color", "#ED9D00");
    Blynk.setProperty(V15, "color", "#ED9D00");
    Blynk.setProperty(V16, "color", "#ED9D00");
    Blynk.setProperty(V32, "color", "#ED9D00");
  }

  Blynk.virtualWrite(V25, pumps[pumpSettingChoice1].mlDailyMode); //ml
  Blynk.virtualWrite(V26, pumps[pumpSettingChoice1].dlDailyMode); //dl
  String mon = String("Mo ");
  String tue = String("Tu ");
  String wed = String("We ");
  String thu = String("Th ");
  String fri = String("Fr ");
  String sat = String("Sa ");
  String sun = String("Su");
  if(pumps[pumpSettingChoice1].Mon==0){
    mon = "";
  }
  if(pumps[pumpSettingChoice1].Tue==0){
    tue = "";
  }
  if(pumps[pumpSettingChoice1].Wed==0){
    wed = "";
  }
  if(pumps[pumpSettingChoice1].Thu==0){
    thu = "";
  }
  if(pumps[pumpSettingChoice1].Fri==0){
    fri = "";
  }
  if(pumps[pumpSettingChoice1].Sat==0){
    sat = "";
  }
  if(pumps[pumpSettingChoice1].Sun==0){
    sun = "";
  }
  String printDay = String(mon) + tue + wed + thu + fri + sat + sun;
  Blynk.virtualWrite(V24, printDay);
  /* String rowTime;
  if(EEPROM.read(25)<10 && EEPROM.read(26)<10 )
  {
    String rowTime = String("0") + EEPROM.read(25) + ":" + "0"+ EEPROM.read(26);
  }
  else if (EEPROM.read(25)<10)
  {
    String rowTime = String("0") + EEPROM.read(25) + ":" +  EEPROM.read(26);
  }
  else if(EEPROM.read(26)<10 )
  {
    String rowTime = String(EEPROM.read(25)) + ":" + "0"+ EEPROM.read(26);
  }
  else
  {
    String rowTime = String(EEPROM.read(25)) + ":" + EEPROM.read(26);
  }*/
}

void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);

  for(int actualPump = 0; actualPump < numberOfPump; ++actualPump) {
    EEPROM.get(EEPROM_pumpSize * actualPump, pumps[actualPump]); 
  }

  for(int actualPump = 0; actualPump < numberOfPump; ++actualPump) {
    Serial.print("\n\n\tpumpPrint ");
    Serial.println(actualPump);
    pumpPrint(pumps[actualPump]); 
  }

  setupLeds();
  setupButtons();

  pinMode(pump1dir,OUTPUT);
  pinMode(pump2dir,OUTPUT);
  pinMode(motor1pin,OUTPUT);
  pinMode(motor2pin,OUTPUT);
  digitalWrite(pump1dir,LOW);
  digitalWrite(pump2dir,LOW);

  Blynk.begin(auth, ssid, pass);
  setSyncInterval(10 * 60);
  timer.setInterval(500L,getTime);
  timer.setInterval(100L, pumpAllValue);
  timer.setInterval(100L, pumpAllValue2);
  timer.setInterval(100L,printActSetVal);
  timer.setInterval(500L, timeTesting);
  timer.setInterval(500L, pumpStop);
}

void loop()
{   
  Blynk.run();
  timer.run();

  for(int actualPump = 0; actualPump < numberOfPump; ++actualPump) {
    if(pumps[actualPump].changed) {
      pumps[actualPump].changed = false;
      EEPROM.put(EEPROM_pumpSize * actualPump, pumps[actualPump]); 
      EEPROM.commit();
    }
  }
}