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
WidgetLED led1(V5);
WidgetLCD lcd(V7);
WidgetLCD clockLcd(V3);
WidgetRTC rtc;
WidgetTable table;
BLYNK_ATTACH_WIDGET(table, V0);
int rowIndex = 0;
int oneFor = 0;
int8_t pumpSettingChoice = 1;

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}
void sendEvent() {
  // adding 1 row to table every second
  Blynk.virtualWrite(V0, "add", rowIndex, "My Event", rowIndex);

  //highlighting latest added row in table
  Blynk.virtualWrite(V0, "pick", 10);

  rowIndex++;
}

void getTime()
{
    String currentTime = String(hour()) + ":" + minute() + ":" + second();
    clockLcd.print(2,0,currentTime);
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
  if(EEPROM.read(7) == 1)
  {
    if(EEPROM.read(6) == 2)
    {
      if(EEPROM.read(1) == hour() && EEPROM.read(2) == minute()	&& EEPROM.read(3) == second()) //pump1 multiple mode
      { 
        uint8_t pumpNumber = 1;
        startPumpInMultipleMode(EEPROM.read(0),EEPROM.read(4),EEPROM.read(5),EEPROM.read(1),EEPROM.read(2),EEPROM.read(3),pumpNumber);
        oneFor = 0;
      }
    }
    else if (EEPROM.read(6) == 1)
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

//setting menu pump choice
BLYNK_WRITE(V21)
 {
  switch (param.asInt())
  {
    case 1: 
      pumpSettingChoice == 1;   
      break;
    case 2: 
      pumpSettingChoice == 2;
      break;
    default:
  }
}

// PUMP 1 MULTIPLE TIME INPUT START

BLYNK_WRITE(V9) 
{
  long startTimeInSecs = param[0].asLong();
  Serial.println(startTimeInSecs);
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
  EEPROM.commit();
}

// PUMP 1 MULTIPLE

BLYNK_WRITE(V19) 
{
  int multiple = param.asInt();
  EEPROM.write(0,multiple);
  EEPROM.commit();
}

//mililiter pump 1 multiple mode

BLYNK_WRITE(V15) 
{
  int milliliters = param.asInt();
  EEPROM.write(5,milliliters);
  EEPROM.commit();
}

//deciliter pump 1 multiple mode

BLYNK_WRITE(V16) 
{
  if(pumpSettingChoice == 1)
  {
    int decilliters = param.asInt();
    EEPROM.write(4,decilliters);
  }
  EEPROM.commit();
}

void printActSetVal()
{
  if(pumpSettingChoice == 1)
  {
    Blynk.virtualWrite(V18, EEPROM.read(5));
    Blynk.virtualWrite(V17, EEPROM.read(4));
    Blynk.virtualWrite(V20, EEPROM.read(0));
  }
  else if (pumpSettingChoice == 2)
  {
    Blynk.virtualWrite(V18, EEPROM.read(13));
    Blynk.virtualWrite(V17, EEPROM.read(12));
    Blynk.virtualWrite(V20, EEPROM.read(8));
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
    led1.off();
    Blynk.begin(auth, ssid, pass);
    setSyncInterval(10 * 60);
    timer.setInterval(500L,getTime);
    timer.setInterval(50L, timeTesting);
    timer.setInterval(1000L, sendEvent);
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