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

int8_t pumpSettingChoice = 1;
int8_t pumpSettingChoice1 = 1;
long stopPump1 = 0;
long stopPump2 = 0;
static const uint8_t pump1dir = 16;
static const uint8_t pump2dir = 0;
static const uint8_t motor1pin = 17;
static const uint8_t motor2pin = 4;

#include "data.hpp"
#include "blynk_functions.hpp"


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
  long timeInSecond = 0;
  uint16_t amount = ((dl*100)+ml); 
  uint16_t constTime = 1000;

  HOUR *= 3600;
  MINUTE *= 60;
  

  if(pump1.modeSelect == 2)
  {
    timeInSecond = HOUR+MINUTE+SECOND+pump1.mutliple*3600;
    SECOND = timeInSecond % 60;
    timeInSecond /= 60;
    MINUTE = timeInSecond % 60;
    timeInSecond /= 60;
    HOUR = timeInSecond % 24;
  }
  else if(pump2.modeSelect == 2)
  {
    timeInSecond = HOUR+MINUTE+SECOND+pump2.mutliple*3600;
    SECOND = timeInSecond % 60;
    timeInSecond /= 60;
    MINUTE = timeInSecond % 60;
    timeInSecond /= 60;
    HOUR = timeInSecond % 24;
  }
  else if(pump1.modeSelect == 1) {}
  else if(pump2.modeSelect == 1) {}
  else {}
  
  // EEPROM write
  if(pump1.modeSelect == 2)
  {
    EEPROM.write(1, HOUR);
    EEPROM.write(2, MINUTE);
    EEPROM.write(3, SECOND);

  }
  else if(pump2.modeSelect == 2)
  {
    EEPROM.write(9, HOUR);
    EEPROM.write(10, MINUTE);
    EEPROM.write(11, SECOND);

  }
  else if(pump1.modeSelect == 1)
  {
    EEPROM.write(25, HOUR);
    EEPROM.write(26, MINUTE);
    EEPROM.write(27, SECOND);

  }
  else if(pump2.modeSelect == 1)
  {
    EEPROM.write(21, HOUR);
    EEPROM.write(22, MINUTE);
    EEPROM.write(23, SECOND);
  }
  else{}
  EEPROM.commit();
  

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
  Serial.println(pump1.mutliple);
  Serial.println(EEPROM.read(7));
  Serial.println();
  if(pump1.pumpStatus == 1 || pump2.pumpStatus == 1)
  {
    if(pump1.modeSelect == 2 || pump2.modeSelect == 2)
    {
      if(pump1.hourPeriodMode == hour() && pump1.minutePeriodMode == minute()	&& pump1.secondPeriodMode == second()) //pump1 period mode
      { 
        uint8_t pumpNumber = 1;
        pumpStart(pump1.dlPeriodMode,pump1.mlPeriodMode,pump1.hourPeriodMode,pump1.minutePeriodMode,pump1.secondPeriodMode,pumpNumber);
      }
      if(pump2.hourPeriodMode == hour() && pump2.minutePeriodMode == minute()	&& pump2.secondPeriodMode == second()) //pump2 period mode
      { 
        uint8_t pumpNumber = 2;
        pumpStart(pump2.dlPeriodMode,pump2.mlPeriodMode,pump2.hourPeriodMode,pump2.minutePeriodMode,pump2.secondPeriodMode,pumpNumber);
      }
    }
    else if (pump1.modeSelect == 1 || pump2.modeSelect == 1)
    {

    }
    else{
      Serial.println("Please select mode for pump 1");
    }
  }
}


void printActSetVal()
{
  if(pumpSettingChoice == 1)
  {
    Blynk.virtualWrite(V18, EEPROM.read(5));
    Blynk.virtualWrite(V17, EEPROM.read(4));
    Blynk.virtualWrite(V20, EEPROM.read(0));
    if(EEPROM.read(1)<10 && EEPROM.read(2)<10 )
    {
      String rowTime = String("0") + EEPROM.read(1) + ":" + "0"+ EEPROM.read(2);
      Blynk.virtualWrite(V6, rowTime);
    }
    else if (EEPROM.read(1)<10)
    {
      String rowTime = String("0") + EEPROM.read(1) + ":" +  EEPROM.read(2);
      Blynk.virtualWrite(6, rowTime);
    }
    else if(EEPROM.read(2)<10 )
    {
      String rowTime = String(EEPROM.read(1)) + ":" + "0"+ EEPROM.read(2);
      Blynk.virtualWrite(6, rowTime);
    }
    else
    {
      String rowTime = String(EEPROM.read(1)) + ":" + EEPROM.read(2);
      Blynk.virtualWrite(6, rowTime);
    }
  }
  else if (pumpSettingChoice == 2)
  {
    Blynk.virtualWrite(V18, EEPROM.read(13));
    Blynk.virtualWrite(V17, EEPROM.read(12));
    Blynk.virtualWrite(V20, EEPROM.read(8));
    if(EEPROM.read(9)<10 && EEPROM.read(10)<10 )
    {
      String rowTime = String("0") + EEPROM.read(9) + ":" + "0"+ EEPROM.read(10);
      Blynk.virtualWrite(V6, rowTime);
    }
    else if (EEPROM.read(9)<10)
    {
      String rowTime = String("0") + EEPROM.read(9) + ":" +  EEPROM.read(10);
      Blynk.virtualWrite(V6, rowTime);
    }
    else if(EEPROM.read(10)<10 )
    {
      String rowTime = String(EEPROM.read(9)) + ":" + "0"+ EEPROM.read(10);
      Blynk.virtualWrite(V6, rowTime);
    }
    else
    {
      String rowTime = String(EEPROM.read(9)) + ":" + EEPROM.read(10);
      Blynk.virtualWrite(V6, rowTime);
    }
  }
  else
  {
    Blynk.virtualWrite(V18," ");
    Blynk.virtualWrite(V17," ");
    Blynk.virtualWrite(V20," ");
  }

  if(pumpSettingChoice1 == 1)
  {
    String Mon = String("Mo ");
    String Tue = String("Tu ");
    String Wed = String("We ");
    String Thu = String("Th ");
    String Fri = String("Fr ");
    String Sat = String("Sa ");
    String Sun = String("Su");
    if(EEPROM.read(28)==0)
    {
      Mon = "";
    }
    if(EEPROM.read(29)==0)
    {
      Tue = "";
    }
    if(EEPROM.read(30)==0)
    {
      Wed = "";
    }
    if(EEPROM.read(31)==0)
    {
      Thu = "";
    }
    if(EEPROM.read(32)==0)
    {
      Fri = "";
    }
    if(EEPROM.read(33)==0)
    {
      Sat = "";
    }
    if(EEPROM.read(34)==0)
    {
      Sun = "";
    }

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


    String printDay = String(Mon) + Tue + Wed + Thu + Fri + Sat + Sun;
    Blynk.virtualWrite(V24, printDay);
  }
  else if(pumpSettingChoice1 == 2)
  {
    String Mon = String("Mon");
    String Tue = String("Tue");
    String Wed = String("Wed");
    String Thu = String("Thu");
    String Fri = String("Fri");
    String Sat = String("Sat");
    String Sun = String("Sun");
    if(EEPROM.read(35)==0)
    {
      Mon = "   ";
    }
    if(EEPROM.read(36)==0)
    {
      Tue = "   ";
    }
    if(EEPROM.read(37)==0)
    {
      Wed = "   ";
    }
    if(EEPROM.read(38)==0)
    {
      Thu = "   ";
    }
    if(EEPROM.read(39)==0)
    {
      Fri = "   ";
    }
    if(EEPROM.read(40)==0)
    {
      Sat = "   ";
    }
    if(EEPROM.read(41)==0)
    {
      Sun = "   ";
    }
    String printDay = String(Mon) + " " + Tue + " " + Wed + " " + Thu + " " + Fri + " " + Sat + " " + Sun;
    Blynk.virtualWrite(V24, printDay);
  }
  else
  {
    String errorMesseage = String("Please select pump");
    Blynk.virtualWrite(V24, errorMesseage);
  }
}


void setup()
{
  EEPROM.begin(512);

  EEPROM.get(EEPROM_pumpSize * 0, pump1);
  EEPROM.get(EEPROM_pumpSize * 1, pump2);

 
  setupLeds();
  setupButtons();

  pinMode(pump1dir,OUTPUT);
  pinMode(pump2dir,OUTPUT);
  pinMode(motor1pin,OUTPUT);
  pinMode(motor2pin,OUTPUT);
  digitalWrite(pump1dir,LOW);
  digitalWrite(pump2dir,LOW);

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  setSyncInterval(10 * 60);
  timer.setInterval(500L,getTime);
  timer.setInterval(1000L, pumpAllValue);
  timer.setInterval(1000L, pumpAllValue2);
  timer.setInterval(100L,printActSetVal);
  timer.setInterval(500L, timeTesting);
 // timer.setInterval(500L, pumpStop);
}

void loop()
{   

  Blynk.run();
  timer.run();

  if(pump1.changed) {
    pump1.changed = false;
    EEPROM.put(0, pump1);    
  }
  if(pump2.changed) {
    pump2.changed = false;
    EEPROM.put(EEPROM_pumpSize, pump2);    
  }
}