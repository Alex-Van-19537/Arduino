#include "RTClib.h"
#include <Wire.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Не намирам RTC модул!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC е загубил захранване, сверяване...");
    // Сверява времето с момента на компилиране на кода
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
}

void loop() {
  DateTime now = rtc.now();

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}