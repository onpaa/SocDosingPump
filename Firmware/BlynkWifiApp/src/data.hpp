#pragma once


uint8_t periodSettingChoice;
uint8_t dayliSettingChoice;

struct Pump
{
  uint8_t mutliple;

  uint8_t hourPeriodMode;
  uint8_t minutePeriodMode;
  uint8_t secondPeriodMode;

  uint8_t hourDailyMode;
  uint8_t minuteDailyMode;
  uint8_t secondDailyMode;

  uint8_t dlPeriodMode;
  uint8_t mlPeriodMode;

  uint8_t dlDailyMode;
  uint8_t mlDailyMode;

  uint8_t modeSelect;
  uint8_t pumpStatus;

  uint8_t Mon;
  uint8_t Tue;
  uint8_t Wed;
  uint8_t Thu;
  uint8_t Fri;
  uint8_t Sat;
  uint8_t Sun;

  bool changed = false;
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
static const int EEPROM_changed = 20;

static const int EEPROM_pumpSize = 30;


void pumpPrint(Pump & p) {
  Serial.println(p.mutliple);

  Serial.println(p.hourPeriodMode);
  Serial.println(p.minutePeriodMode);
  Serial.println(p.secondPeriodMode);

  Serial.println(p.hourDailyMode);
  Serial.println(p.minuteDailyMode);
  Serial.println(p.secondDailyMode);

  Serial.println(p.dlPeriodMode);
  Serial.println(p.mlPeriodMode);

  Serial.println(p.dlDailyMode);
  Serial.println(p.mlDailyMode);

  Serial.println(p.modeSelect);
  Serial.println(p.pumpStatus);

  Serial.println(p.Mon);
  Serial.println(p.Tue);
  Serial.println(p.Wed);
  Serial.println(p.Thu);
  Serial.println(p.Fri);
  Serial.println(p.Sat);
  Serial.println(p.Sun);

  Serial.println(p.changed);
}