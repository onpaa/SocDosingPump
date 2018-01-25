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

struct pump
{
  uint16_t mutliple;

  uint16_t hourPeriodMode;
  uint16_t minutePeriodMode;
  long secondPeriodMode;

  uint16_t hourDailyMode;
  uint16_t minuteDailyMode;
  long secondDailyMode;

  uint16_t dlPeriodMode;
  uint16_t mlPeriodMode;

  uint16_t dlDailyMode;
  uint16_t mlDailyMode;

  uint8_t modeSelect;
  int8_t pumpStatus;

  int8_t Mon;
  int8_t Tue;
  int8_t Wed;
  int8_t Thu;
  int8_t Fri;
  int8_t Sat;
  int8_t Sun;

};

static const int EEPROM_multiple = 0;
static const int EEPROM_hourPeriodMode = 1;
static const int EEPROM_minutePeriodMode = 2;
static const int EEPROM_secondPeriodMode = 3;
static const int EEPROM_hourDailyMode = 4;
static const int EEPROM_minuteDailyMode = 5;
static const int EEPROM_secondDailyMode = 6;
static const int EEPROM_dlPeriodMode = 7;
static const int EEPROM_mlPeriodMode = 8;
static const int EEPROM_dlDailyMode = 9;
static const int EEPROM_mlDailyMode = 10;
static const int EEPROM_modeSelect = 11;
static const int EEPROM_pumpStatus = 12;
static const int EEPROM_Mon = 13;
static const int EEPROM_Tue = 14;
static const int EEPROM_Wed = 15;
static const int EEPROM_Thu = 16;
static const int EEPROM_Fri = 17;
static const int EEPROM_Sat = 18;
static const int EEPROM_Sun = 19;

static const int EEPROM_TOTAL = 30;

void pumpInitFromEeprom(pump & p, const int numOfPump = 0) {
	const int startAddress = EEPROM_TOTAL * numOfPump;
	  
	p.multiple = EEPROM.read(startAddress + EEPROM_multiple);
	p.hourPeriodMode = EEPROM.read(startAddress + EEPROM_hourPeriodMode);
	p.minutePeriodMode = EEPROM.read(startAddress + EEPROM_minutePeriodMode);
	p.secondPeriodMode = EEPROM.read(startAddress + EEPROM_secondPeriodMode);
	p.hourDailyMode = EEPROM.read(startAddress + EEPROM_hourDailyMode);
	p.minuteDailyMode = EEPROM.read(startAddress + EEPROM_minuteDailyMode);
	p.secondDailyMode = EEPROM.read(startAddress + EEPROM_secondDailyMode);
	p.dlPeriodMode = EEPROM.read(startAddress + EEPROM_dlPeriodMode);
	p.mlPeriodMode = EEPROM.read(startAddress + EEPROM_mlPeriodMode);
	p.dlDailyMode = EEPROM.read(startAddress + EEPROM_dlDailyMode);
	p.mlDailyMode = EEPROM.read(startAddress + EEPROM_mlDailyMode);
	p.modeSelect = EEPROM.read(startAddress + EEPROM_modeSelect);
	p.pumpStatus = EEPROM.read(startAddress + EEPROM_pumpStatus);
	p.Mon = EEPROM.read(startAddress + EEPROM_Mon);
	p.Tue = EEPROM.read(startAddress + EEPROM_Tue);
	p.Wed = EEPROM.read(startAddress + EEPROM_Wed);
	p.Thu = EEPROM.read(startAddress + EEPROM_Thu);
	p.Fri = EEPROM.read(startAddress + EEPROM_Fri);
	p.Sun = EEPROM.read(startAddress + EEPROM_Sun);
	p.Sat = EEPROM.read(startAddress + EEPROM_Sat);
}



// pump pump1
// {
//   EEPROM.read(0), // multiple

//   EEPROM.read(1), //hourPeriodMode
//   EEPROM.read(2), //minutePeriodMode
//   EEPROM.read(3), //secondPeriodMode

//   EEPROM.read(25), //hourDailyMode
//   EEPROM.read(26), //minuteDailyMode
//   EEPROM.read(27), //secondDailyMode

//   EEPROM.read(4), //dlPeriodMode
//   EEPROM.read(5), // mlPeriodMode

//   EEPROM.read(16), //dlDailyMode
//   EEPROM.read(17), //mlDailyMode

//   EEPROM.read(6), //modeSelect
//   EEPROM.read(7), //pumpStatus

//   EEPROM.read(28), //MOn
//   EEPROM.read(29),
//   EEPROM.read(30),
//   EEPROM.read(31),
//   EEPROM.read(32),
//   EEPROM.read(33),
//   EEPROM.read(34) //sun
// };

// pump pump2
// {
//   EEPROM.read(8),

//   EEPROM.read(9),
//   EEPROM.read(10),
//   EEPROM.read(11),

//   EEPROM.read(21),
//   EEPROM.read(22),
//   EEPROM.read(23),

//   EEPROM.read(12),
//   EEPROM.read(13),

//   EEPROM.read(18),
//   EEPROM.read(19),

//   EEPROM.read(14),
//   EEPROM.read(15),

//   EEPROM.read(35),
//   EEPROM.read(36),
//   EEPROM.read(37),
//   EEPROM.read(38),
//   EEPROM.read(39),
//   EEPROM.read(40),
//   EEPROM.read(41)
// };

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

BLYNK_WRITE(V1)
{ 
    int pinValue = param.asInt(); 
    digitalWrite(L_B, pinValue);
    if(param.asInt())
    {
      Serial.println("Pump 1 on");
      EEPROM.write(7,1);
    }
    else{
      Serial.println("Pump 1 off");
      EEPROM.write(7,0);
    }
    EEPROM.commit();
}
BLYNK_WRITE(V13)
{ 
    int pinValue = param.asInt(); 
    digitalWrite(L_G, pinValue);
    if(param.asInt())
    {
      Serial.println("Pump 2 on");
      EEPROM.write(15,1);
    }
    else{
      Serial.println("Pump 2 off");
      EEPROM.write(15,0);
    }
    EEPROM.commit();
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
BLYNK_WRITE(V8)
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
//setting PERIOD menu pump choice
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

//setting DAY DOSING menu pump choice
BLYNK_WRITE(V23)
 {
  switch (param.asInt())
  {
    case 1: 
      pumpSettingChoice1 = 1;   
      break;
    case 2: 
      pumpSettingChoice1 = 2;
      break;
    default:
      pumpSettingChoice1 = 3;
  }
}
// PUMP MULTIPLE TIME INPUT START

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

// PUMP DAY DOSING TIME INPUT
BLYNK_WRITE(V7) 
{
  if (pumpSettingChoice1 == 1)  
  {
    TimeInputParam t(param);
    if (t.hasStartTime())
    {
      EEPROM.write(25,t.getStartHour()); //hour
      EEPROM.write(26,t.getStartMinute()); //minute
      EEPROM.write(27,t.getStartSecond()); //second
    }
    else{}

    if (t.isWeekdaySelected(1)) 
    {
      EEPROM.write(28,1);
      
    }
    else
    {
      EEPROM.write(28,0);
    }

    if (t.isWeekdaySelected(2)) 
    {
      EEPROM.write(29,1);
     
    }
    else
    {
      EEPROM.write(29,0);
    }

    if (t.isWeekdaySelected(3)) 
    {
      EEPROM.write(30,1);
      
    }
    else
    {
      EEPROM.write(30,0);
    }

    if (t.isWeekdaySelected(4)) 
    {
      EEPROM.write(31,1);
     
    }
    else
    {
      EEPROM.write(31,0);
    }

    if (t.isWeekdaySelected(5)) 
    {
      EEPROM.write(32,1);
      
    }
    else
    {
      EEPROM.write(32,0);
    }

    if (t.isWeekdaySelected(6)) 
    {
      EEPROM.write(33,1);
      
    }
    else
    {
      EEPROM.write(33,0);
    }

    if (t.isWeekdaySelected(7)) 
    {
      EEPROM.write(34,1);
      
    }
    else
    {
      EEPROM.write(34,0);
    }

    EEPROM.commit();
    Serial.println(String(EEPROM.read(28)) +  ":" + EEPROM.read(29) + ":" + EEPROM.read(30) +  ":" + EEPROM.read(31) + ":" + EEPROM.read(32) +  ":" + EEPROM.read(33) + ":" + EEPROM.read(34));
  }
  else if (pumpSettingChoice1 == 2)  
  {
    TimeInputParam t(param);
    if (t.hasStartTime())
    {
      EEPROM.write(21,t.getStartHour()); //hour
      EEPROM.write(22,t.getStartMinute()); //minute
      EEPROM.write(23,t.getStartSecond()); //second
    }
    else{}

    if (t.isWeekdaySelected(1)) 
    {
      EEPROM.write(35,1);
    }
    else
    {
      EEPROM.write(35,0);
    }

    if (t.isWeekdaySelected(2)) 
    {
      EEPROM.write(36,1);
    }
    else
    {
      EEPROM.write(36,0);
    }

    if (t.isWeekdaySelected(3)) 
    {
      EEPROM.write(37,1);
    }
    else
    {
      EEPROM.write(37,0);
    }

    if (t.isWeekdaySelected(4)) 
    {
      EEPROM.write(38,1);
    }
    else
    {
      EEPROM.write(38,0);
    }

    if (t.isWeekdaySelected(5)) 
    {
      EEPROM.write(39,1);
    }
    else
    {
      EEPROM.write(39,0);
    }

    if (t.isWeekdaySelected(6)) 
    {
      EEPROM.write(40,1);
    }
    else
    {
      EEPROM.write(40,0);
    }

    if (t.isWeekdaySelected(7)) 
    {
      EEPROM.write(41,1);
    }
    else
    {
      EEPROM.write(41,0);
    }
    EEPROM.commit();
    Serial.println(String(EEPROM.read(35)) +  ":" + EEPROM.read(36) + ":" + EEPROM.read(37) +  ":" + EEPROM.read(38) + ":" + EEPROM.read(39) +  ":" + EEPROM.read(40) + ":" + EEPROM.read(41));
  }
  else
  {
    String errorMesseage = String("Please select pump");
    Blynk.virtualWrite(V24, errorMesseage);
  }
}




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

//pump1 calibration
BLYNK_WRITE(V27) 
{ 
    if(param.asInt())
    {
      digitalWrite(motor1pin,HIGH);
    }
    else
    {
      digitalWrite(motor1pin, LOW);
    }
}
//pump2 calibration
BLYNK_WRITE(V28) 
{ 
   if(param.asInt())
    {
      digitalWrite(motor2pin,HIGH);
    }
    else
    {
      digitalWrite(motor2pin, LOW);
    }
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
}