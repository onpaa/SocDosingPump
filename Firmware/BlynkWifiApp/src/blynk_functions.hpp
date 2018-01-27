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
      pumps[0].pumpStatus = 1;
    }
    else{
      Serial.println("Pump 1 off");
      pumps[0].pumpStatus = 0;
    }
    pumps[0].changed = true;
}
BLYNK_WRITE(V13)
{ 
    int pinValue = param.asInt(); 
    digitalWrite(L_G, pinValue);
    if(param.asInt())
    {
      Serial.println("Pump 2 on");
      pumps[1].pumpStatus = 1;
    }
    else{
      Serial.println("Pump 2 off");
      pumps[1].pumpStatus = 0;
    }
    pumps[1].changed = true;
}
//pump1 mode select
BLYNK_WRITE(V2)
 {
  switch (param.asInt())
  {
    case 1: 
      pumps[0].modeSelect = 1;    
      break;
    case 2: 
      pumps[0].modeSelect = 2;
      break;
    default:
      pumps[0].modeSelect = 1;
  }
  pumps[0].changed = true;
}

//pump2 mode select
BLYNK_WRITE(V8)
 {
  switch (param.asInt())
  {
    case 1: 
      pumps[1].modeSelect = 1;   
      break;
    case 2: 
      pumps[1].modeSelect = 2;
      break;
    default:
      pumps[1].modeSelect = 1;
  }
  pumps[1].changed = true;
}
//setting PERIOD menu pump choice
BLYNK_WRITE(V21)
 {
  switch (param.asInt())
  {
    case 1: 
      pumpSettingChoice = 0;   
      break;
    case 2: 
      pumpSettingChoice = 1;
      break;
    default:
      pumpSettingChoice = 0;
  }
}

//setting DAY DOSING menu pump choice
BLYNK_WRITE(V23)
 {
  switch (param.asInt())
  {
    case 1: 
      pumpSettingChoice1 = 0;   
      break;
    case 2: 
      pumpSettingChoice1 = 1;
      break;
    default:
      pumpSettingChoice1 = 0;
  }
}
// PUMP MULTIPLE TIME INPUT START

BLYNK_WRITE(V9) 
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

  pumps[pumpSettingChoice].hourPeriodMode = h;
  pumps[pumpSettingChoice].minutePeriodMode = m;
  pumps[pumpSettingChoice].secondPeriodMode = s;
}

// PUMP DAY DOSING TIME INPUT
BLYNK_WRITE(V7) 
{
  TimeInputParam t(param);
  if (t.hasStartTime()) {
    pumps[pumpSettingChoice1].hourDailyMode = t.getStartHour();
    pumps[pumpSettingChoice1].minuteDailyMode = t.getStartMinute();
    pumps[pumpSettingChoice1].secondDailyMode = t.getStartSecond();
  }
  
  if (t.isWeekdaySelected(1)) {
    pumps[pumpSettingChoice1].Mon = 1;
  }
  else {
    pumps[pumpSettingChoice1].Mon = 0;
  }
  if (t.isWeekdaySelected(2)) {
    pumps[pumpSettingChoice1].Tue = 1;
  }
  else {
    pumps[pumpSettingChoice1].Tue = 0;
  }
  if (t.isWeekdaySelected(3)) {
    pumps[pumpSettingChoice1].Wed = 1;  
  }
  else {
    pumps[pumpSettingChoice1].Wed = 0;
  }
  if (t.isWeekdaySelected(4)) {
    pumps[pumpSettingChoice1].Thu = 1;
  }
  else {
    pumps[pumpSettingChoice1].Thu = 0;
  }
  if (t.isWeekdaySelected(5)) {
    pumps[pumpSettingChoice1].Fri = 1;
  }
  else {
    pumps[pumpSettingChoice1].Fri = 0;
  }
  if (t.isWeekdaySelected(6))  {
    pumps[pumpSettingChoice1].Sat = 1;
  }
  else {
    pumps[pumpSettingChoice1].Sat = 0;
  }
  if (t.isWeekdaySelected(7)) {
    pumps[pumpSettingChoice1].Sun = 1;
  }
  else{
    pumps[pumpSettingChoice1].Sun = 0;
  }
}

// save button periodical setting
BLYNK_WRITE(V29)
{  
    if(param.asInt())
    {
      Serial.println("Data in periodical setting has been saved");
      pumps[pumpSettingChoice].changed = true;
    }
}

// save button daily setting
BLYNK_WRITE(V30)
{  
    if(param.asInt())
    {
      Serial.println("Data in dayli setting has been saved");
      pumps[pumpSettingChoice1].changed = true;
    }
}

// period mode PERIOD
BLYNK_WRITE(V19) 
{
  pumps[pumpSettingChoice].mutliple = param.asInt();
}

// period dl set
BLYNK_WRITE(V16) 
{
  pumps[pumpSettingChoice].dlPeriodMode = param.asInt();
}

// daily dl set
BLYNK_WRITE(V14) 
{
  pumps[pumpSettingChoice1].dlDailyMode = param.asInt();
}

// period ml set
BLYNK_WRITE(V15) 
{
  pumps[pumpSettingChoice].mlPeriodMode = param.asInt();
}

// daily ml set
BLYNK_WRITE(V31) 
{
  pumps[pumpSettingChoice1].mlDailyMode = param.asInt();
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