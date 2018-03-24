#pragma once

BLYNK_CONNECTED() 
{
  rtc.begin();
}

BLYNK_WRITE(V0)
{ 
  //int pinValue = param.asInt(); 
  //digitalWrite(L_B, pinValue);
  if(param.asInt())
  {
    for(int actualPump = 0; actualPump < numberOfPump; ++actualPump) {
    Serial.print("\n\n\tpumpPrint ");
    Serial.println(actualPump);
    pumpPrint(pumps[actualPump]); 
    }
  }
}
BLYNK_WRITE(V1)
{ 
    //int pinValue = param.asInt(); 
    //digitalWrite(L_B, pinValue);
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
    //int pinValue = param.asInt(); 
    //digitalWrite(L_G, pinValue);
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
      pumps[0].modeSelect = 0; 
      Serial.println("Daily dosing");
      break;
    case 2: 
      pumps[0].modeSelect = 1;
      Serial.println("Period dosing");
      break;
    default:
      pumps[0].modeSelect = 0;
      Serial.println("Daily dosing default case");
  }
  pumps[0].changed = true;
}

//pump2 mode select
BLYNK_WRITE(V8)
 {
  switch (param.asInt())
  {
    case 1: 
      pumps[1].modeSelect = 0;  
      Serial.println("Daily dosing"); 
      break;
    case 2: 
      pumps[1].modeSelect = 1;
      Serial.println("Period dosing");
      break;
    default:
      pumps[1].modeSelect = 0;
      Serial.println("Daily dosing default case");
  }
  pumps[1].changed = true;
}
BLYNK_WRITE(V32)
 {
  switch (param.asInt())
  {
    case 1: 
      periodSettingChoice = 0;   
      break;
    case 2: 
      periodSettingChoice = 1;  
      break;
    default:
      periodSettingChoice = 0;  
  }
  pumps[periodSettingChoice].changed = true;
}
//setting DAY DOSING menu pump choice
BLYNK_WRITE(V23)
 {
  switch (param.asInt())
  {
    case 1: 
      dayliSettingChoice = 0;  
      break;
    case 2: 
      dayliSettingChoice = 1;  
      break;
    default:
      dayliSettingChoice = 0;  
  }
  pumps[dayliSettingChoice].changed = true;
}
// PUMP period TIME INPUT START

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

  pumps[periodSettingChoice].hourPeriodMode = h;
  pumps[periodSettingChoice].minutePeriodMode = m;
  pumps[periodSettingChoice].secondPeriodMode = s;
  pumps[periodSettingChoice].changed = true;
}

// PUMP DAY DOSING TIME INPUT
BLYNK_WRITE(V7) 
{
  TimeInputParam t(param);
  if (t.hasStartTime()) {
    pumps[dayliSettingChoice].hourDailyMode = t.getStartHour();
    pumps[dayliSettingChoice].minuteDailyMode = t.getStartMinute();
    pumps[dayliSettingChoice].secondDailyMode = t.getStartSecond();
  }
  
  if (t.isWeekdaySelected(1)) {
    pumps[dayliSettingChoice].Mon = 1;
  }
  else {
    pumps[dayliSettingChoice].Mon = 0;
  }
  if (t.isWeekdaySelected(2)) {
    pumps[dayliSettingChoice].Tue = 1;
  }
  else {
    pumps[dayliSettingChoice].Tue = 0;
  }
  if (t.isWeekdaySelected(3)) {
    pumps[dayliSettingChoice].Wed = 1;  
  }
  else {
    pumps[dayliSettingChoice].Wed = 0;
  }
  if (t.isWeekdaySelected(4)) {
    pumps[dayliSettingChoice].Thu = 1;
  }
  else {
    pumps[dayliSettingChoice].Thu = 0;
  }
  if (t.isWeekdaySelected(5)) {
    pumps[dayliSettingChoice].Fri = 1;
  }
  else {
    pumps[dayliSettingChoice].Fri = 0;
  }
  if (t.isWeekdaySelected(6))  {
    pumps[dayliSettingChoice].Sat = 1;
  }
  else {
    pumps[dayliSettingChoice].Sat = 0;
  }
  if (t.isWeekdaySelected(7)) {
    pumps[dayliSettingChoice].Sun = 1;
  }
  else{
    pumps[dayliSettingChoice].Sun = 0;
  }
  pumps[dayliSettingChoice].changed = true;
}

// period mode PERIOD
BLYNK_WRITE(V19) 
{
  pumps[periodSettingChoice].mutliple = param.asInt();
  pumps[periodSettingChoice].changed = true;
}

// period dl set
BLYNK_WRITE(V16) 
{
  pumps[periodSettingChoice].dlPeriodMode = param.asInt();
  pumps[periodSettingChoice].changed = true;
}

// daily dl set
BLYNK_WRITE(V14) 
{
  pumps[dayliSettingChoice].dlDailyMode = param.asInt();
  pumps[dayliSettingChoice].changed = true;
}

// period ml set
BLYNK_WRITE(V15) 
{
  pumps[periodSettingChoice].mlPeriodMode = param.asInt();
  pumps[periodSettingChoice].changed = true;
}

// daily ml set
BLYNK_WRITE(V31) 
{
  pumps[dayliSettingChoice].mlDailyMode = param.asInt();
  pumps[dayliSettingChoice].changed = true;
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