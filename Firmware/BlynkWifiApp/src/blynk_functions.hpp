#pragma once

BLYNK_CONNECTED() 
{
  rtc.begin();
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