#define BLYNK_PRINT Serial

#include <LearningKit.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <EEPROM.h>


char auth[] = "b5ac866ca280448cae7a375658aa923d";

char ssid[] = "WL520GC";
char pass[] = "klemont1";

BlynkTimer timer;
WidgetRTC rtc;

int oneFor = 0;
int8_t pumpSettingChoice = 1;

BLYNK_CONNECTED() 
{
  rtc.begin();
}

void pumpAllValue() 
{
  if (EEPROM.read(7)==1)
  {
    String rowStatus = String("Status                                  ") + "ON";
    Blynk.virtualWrite(V3, rowStatus);
  }
  else if (EEPROM.read(7)==0)
  {
    String rowStatus = String("Status                                  ") + "OFF";
    Blynk.virtualWrite(V3, rowStatus);
  }
  else{}

  if(EEPROM.read(1)<10 && EEPROM.read(2)<10 )
  {
    String rowTime = String("Time                                   ")+ "0" + EEPROM.read(1) + ":" + "0"+ EEPROM.read(2);
    Blynk.virtualWrite(V4, rowTime);
  }
  else if (EEPROM.read(1)<10)
  {
    String rowTime = String("Time                                   ")+ "0" + EEPROM.read(1) + ":" +  EEPROM.read(2);
    Blynk.virtualWrite(V4, rowTime);
  }
  else if(EEPROM.read(2)<10 )
  {
    String rowTime = String("Time                                   ") + EEPROM.read(1) + ":" + "0"+ EEPROM.read(2);
    Blynk.virtualWrite(V4, rowTime);
  }
  else
  {
    String rowTime = String("Time                                    ") + EEPROM.read(1) + ":" + EEPROM.read(2);
    Blynk.virtualWrite(V4, rowTime);
  }

  if (EEPROM.read(4)==0)
  {
    String rowAmount = String("Amount                               ") + EEPROM.read(5) + " ml";
    Blynk.virtualWrite(V5, rowAmount);
  }
  else
  {
    String rowAmount = String("Amount              ") + EEPROM.read(4) + " dl   " + EEPROM.read(5) + " ml       ";
    Blynk.virtualWrite(V5, rowAmount);
  }
}
void pumpAllValue2() 
{
  if (EEPROM.read(15)==1)
  {
    String rowStatus = String("Status                                  ") + "ON";
    Blynk.virtualWrite(V10, rowStatus);
  }
  else if (EEPROM.read(15)==0)
  {
    String rowStatus = String("Status                                  ") + "OFF";
    Blynk.virtualWrite(V10, rowStatus);
  }
  else{}

  if(EEPROM.read(9)<10 && EEPROM.read(10)<10 )
  {
    String rowTime = String("Time                                   ")+ "0" + EEPROM.read(9) + ":" + "0"+ EEPROM.read(10);
    Blynk.virtualWrite(V12, rowTime);
  }
  else if (EEPROM.read(9)<10)
  {
    String rowTime = String("Time                                   ")+ "0" + EEPROM.read(9) + ":" +  EEPROM.read(10);
    Blynk.virtualWrite(V12, rowTime);
  }
  else if(EEPROM.read(10)<10 )
  {
    String rowTime = String("Time                                   ") + EEPROM.read(9) + ":" + "0"+ EEPROM.read(10);
    Blynk.virtualWrite(V12, rowTime);
  }
  else
  {
    String rowTime = String("Time                                    ") + EEPROM.read(9) + ":" + EEPROM.read(10);
    Blynk.virtualWrite(V12, rowTime);
  }

  if (EEPROM.read(12)==0)
  {
    String rowAmount = String("Amount                               ") + EEPROM.read(13) + " ml";
    Blynk.virtualWrite(V11, rowAmount);
  }
  else
  {
    String rowAmount = String("Amount              ") + EEPROM.read(12) + " dl   " + EEPROM.read(13) + " ml       ";
    Blynk.virtualWrite(V11, rowAmount);
  }
}

void getTime()
{
    String currentTime = String(hour()) + ":" + minute() + ":" + second();
    Blynk.virtualWrite(V22, currentTime);
}

void startPumpInMultipleMode(long multiple, long dl, long ml, long hour, int minute, uint16_t second, uint8_t pumpNumber)
{
  uint16_t amount = ((dl*100)+ml); 
  uint16_t constTime = 100;
  long pumpTime = amount*constTime;

  long timeInSecond = 0;
  hour *= 3600;
  minute *= 60;
  multiple *= 3600;
  timeInSecond = hour+minute+second+multiple;

  second = timeInSecond % 60;
  timeInSecond /= 60;
  minute = timeInSecond % 60;
  timeInSecond /= 60;
  hour = timeInSecond % 24;

  if(pumpNumber == 1)
  {
    pumpNumber = L_R;
  }
  else
  {
    pumpNumber = L_Y;
  }

  bool whileEnd = false;
  long preMillis = 0;
  int motorStatus = LOW;
  while(whileEnd == false) //motor on/off
  {
    unsigned long currentMillis = millis();
    Serial.println("ve smycce");
    if(currentMillis - preMillis > pumpTime) 
    {
      preMillis = currentMillis;
      if  (motorStatus == LOW)
      {
       motorStatus = HIGH;
      }
      else
      {
       motorStatus = LOW;
        whileEnd = true;
      }
      digitalWrite(pumpNumber, motorStatus);     
    }
  }
  if(oneFor == 0)
  {
    EEPROM.write(1, hour);
    EEPROM.write(2, minute);
    EEPROM.write(3, second);
    oneFor++;
  }
}

void startPumpInExactMode(){}

void timeTesting()
{ 
  if(EEPROM.read(7) == 1 || EEPROM.read(15) == 1)
  {
    if(EEPROM.read(6) == 2 || EEPROM.read(14) == 2)
    {
      //if((EEPROM.read(1) == hour() && EEPROM.read(2) == minute()	&& EEPROM.read(3) == second()) && 
      //(EEPROM.read(9) == hour() && EEPROM.read(10) == minute()	&& EEPROM.read(11) == second())) //pump1 period mode
      //{ }
      if(EEPROM.read(1) == hour() && EEPROM.read(2) == minute()	&& EEPROM.read(3) == second()) //pump1 period mode
      { 
        uint8_t pumpNumber = 1;
        startPumpInMultipleMode(EEPROM.read(0),EEPROM.read(4),EEPROM.read(5),EEPROM.read(1),EEPROM.read(2),EEPROM.read(3),pumpNumber);
        oneFor = 0;
      }
      if(EEPROM.read(9) == hour() && EEPROM.read(10) == minute()	&& EEPROM.read(11) == second()) //pump2 period mode
      { 
        uint8_t pumpNumber = 2;
        startPumpInMultipleMode(EEPROM.read(8),EEPROM.read(12),EEPROM.read(13),EEPROM.read(9),EEPROM.read(10),EEPROM.read(11),pumpNumber);
        oneFor = 0;
      }
    }
    else if (EEPROM.read(6) == 1 || EEPROM.read(14) == 1)
    {

    }
    else{
      Serial.println("Please select mode for pump 1");
    }
  }
}

BLYNK_WRITE(V1)
{ 
    int pinValue = param.asInt(); 
    digitalWrite(L_B, pinValue);
    if(param.asInt())
    {
      Serial.println("Pump 1 on");
      EEPROM.write(7,1);
      EEPROM.commit();
    }
    else{
      Serial.println("Pump 1 off");
      EEPROM.write(7,0);
      EEPROM.commit();
    }
}
BLYNK_WRITE(V13)
{ 
    int pinValue = param.asInt(); 
    digitalWrite(L_G, pinValue);
    if(param.asInt())
    {
      Serial.println("Pump 2 on");
      EEPROM.write(15,1);
      EEPROM.commit();
    }
    else{
      Serial.println("Pump 2 off");
      EEPROM.write(15,0);
      EEPROM.commit();
    }
}
//pump1 mode select
BLYNK_WRITE(V2)
 {
  switch (param.asInt())
  {
    case 1: 
      EEPROM.write(6,1);    
      break;
    case 2: 
      EEPROM.write(6,2);
      break;
    default:
      Serial.println("Unknown item selected");
  }
  EEPROM.commit();
}

//pump2 mode select
BLYNK_WRITE(V6)
 {
  switch (param.asInt())
  {
    case 1: 
      EEPROM.write(14,1);    
      break;
    case 2: 
      EEPROM.write(14,2);
      break;
    default:
      Serial.println("Unknown item selected");
  }
  EEPROM.commit();
}
//setting menu pump choice
BLYNK_WRITE(V21)
 {
  switch (param.asInt())
  {
    case 1: 
      pumpSettingChoice = 1;   
      break;
    case 2: 
      pumpSettingChoice = 2;
      break;
    default:
      pumpSettingChoice = 3;
  }
}

// PUMP 1 MULTIPLE TIME INPUT START

BLYNK_WRITE(V9) 
{
  if(pumpSettingChoice == 1)
  {
    long startTimeInSecs = param[0].asLong();

    int s = 0, m = 0, h = 0;
    s = startTimeInSecs % 60;
    startTimeInSecs /= 60;
    m = startTimeInSecs % 60;
    startTimeInSecs /= 60;
    h = startTimeInSecs % 24;

    Serial.println();
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.print(":");
    Serial.println(s);

    EEPROM.write(1,h);
    EEPROM.write(2,m);
    EEPROM.write(3,s);
  }
  else if(pumpSettingChoice == 2)
  {
    long startTimeInSecs = param[0].asLong();

    int s = 0, m = 0, h = 0;
    s = startTimeInSecs % 60;
    startTimeInSecs /= 60;
    m = startTimeInSecs % 60;
    startTimeInSecs /= 60;
    h = startTimeInSecs % 24;

    Serial.println();
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.print(":");
    Serial.println(s);

    EEPROM.write(9,h);
    EEPROM.write(10,m);
    EEPROM.write(11,s);
   }
   else{}
  EEPROM.commit();
}

// PUMP MULTIPLE

BLYNK_WRITE(V19) 
{
  if(pumpSettingChoice == 1)
  {
   int multiple = param.asInt();
    EEPROM.write(0,multiple);
  }
  else if(pumpSettingChoice == 2)
  {
    int multiple = param.asInt();
    EEPROM.write(8,multiple);
  }
  else{}
  EEPROM.commit();
}

//mililiter pump multiple mode

BLYNK_WRITE(V15) 
{ 
  if(pumpSettingChoice == 1)
  {
    int milliliters = param.asInt();
    EEPROM.write(5,milliliters);
  }
  else if(pumpSettingChoice == 2)
  {
    int milliliters = param.asInt();
    EEPROM.write(13,milliliters);
  }
  else{}
  EEPROM.commit();
}

//deciliter pump multiple mode

BLYNK_WRITE(V16) 
{
  if(pumpSettingChoice == 1)
  {
    int decilliters = param.asInt();
    EEPROM.write(4,decilliters);
  }
  else if(pumpSettingChoice == 2)
  {
    int decilliters = param.asInt();
    EEPROM.write(12,decilliters);
  }
  else{}
  EEPROM.commit();
}

void printActSetVal()
{
  if(pumpSettingChoice == 1)
  {
    Blynk.virtualWrite(V18, EEPROM.read(5));
    Blynk.virtualWrite(V17, EEPROM.read(4));
    Blynk.virtualWrite(V20, EEPROM.read(0));
    Serial.println(pumpSettingChoice);
  }
  else if (pumpSettingChoice == 2)
  {
    Blynk.virtualWrite(V18, EEPROM.read(13));
    Blynk.virtualWrite(V17, EEPROM.read(12));
    Blynk.virtualWrite(V20, EEPROM.read(8));
    Serial.println(pumpSettingChoice);
  }
  else
  {
    Blynk.virtualWrite(V18," ");
    Blynk.virtualWrite(V17," ");
    Blynk.virtualWrite(V20," ");
  }
}


void setup()
{
    EEPROM.begin(512);
    setupLeds();
    setupButtons();
    
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    setSyncInterval(10 * 60);
    timer.setInterval(500L,getTime);
    timer.setInterval(50L, timeTesting);
    timer.setInterval(1000L, pumpAllValue);
    timer.setInterval(1000L, pumpAllValue2);
    timer.setInterval(500L,printActSetVal);

    if(EEPROM.read(7)==0)
    {
      Blynk.setProperty(V1,"label", "offLabel");
    }
    else
    {
      Blynk.setProperty(V1,"label", "onLabel");
    }
}

void loop()
{   
    timeTesting();
    Blynk.run();
    timer.run();
}