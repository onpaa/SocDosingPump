#define BLYNK_PRINT Serial

#include <LearningKit.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

// AHOJ


char auth[] = "b5ac866ca280448cae7a375658aa923d";

char ssid[] = "SPAVOS_NET";
char pass[] = "m0gu145.";

BlynkTimer timer;
WidgetLED led1(V5);
WidgetLCD lcd(V7);
WidgetLCD clockLcd(V3);
WidgetRTC rtc;

int8_t ledStatus = 1;
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V3, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V3, currentDate);
}
BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

BLYNK_WRITE(V1)
{ 
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    Serial.print("V1 Slider value is: ");
    Serial.println(pinValue);
    digitalWrite(L_B, pinValue);
    if(ledStatus == 1)
    {
        led1.on();
        ledStatus = 0;
    }
    else
    {
        led1.off();
        ledStatus = 1;
    } 

}


void setup()
{
    setupLeds();
    setupButtons();

    Serial.begin(9600);
    led1.off();
    Blynk.begin(auth, ssid, pass);
    lcd.print(2,0,"Davkovat za");
    setSyncInterval(10 * 60);
    timer.setInterval(10000L, clockDisplay);
}
int getTime(int HOUR = 0,int minute = 0,int second = 0)
{
    String currentTime = String(hour()) + ":" + minute() + ":" + second();

}
void loop()
{   
    
    clockLcd.clear();
    clockLcd.print(2,0,currentTime);
    Blynk.run();
    timer.run();
}