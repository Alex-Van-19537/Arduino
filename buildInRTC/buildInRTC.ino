#include "RTC.h"

void setup() {
  Serial.begin(9600);

  // Инициализация на RTC модула
  RTC.begin();
}

void loop() {
  RTCTime currentTime;
  
  // Четене на текущото време
  if (RTC.getTime(currentTime)) {
    Serial.print("Текущо време: ");
    Serial.print(currentTime.getHour());
    Serial.print(":");
    Serial.print(currentTime.getMinutes());
    Serial.print(":");
    Serial.println(currentTime.getSeconds());
  }

  // Използване на голямо забавяне или Sleep mode за енергийна ефективност
  delay(1000); 
}