#include "RTC.h"
#include <WiFiS3.h>

const char ssid[] = "Marko"; 
const char pass[] = "Nadejda123";         

void setup() {
  Serial.begin(9600);
  while(!Serial); 
  
  RTC.begin();

  Serial.print("Свързване с WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nУспешно свързване!");

  Serial.print("Извличане на време от мрежата...");
  
  unsigned long epochTime = 0;
  
  // Чакаме докато модулът успее да вземе валидно време (различно от 0)
  while (epochTime == 0) {
    epochTime = WiFi.getTime();
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nВремето е извлечено!");

  // WiFi.getTime() връща времето по Гринуич (UTC). 
  // За България добавяме отместването: 10800 секунди за лятно време (UTC+3) 
  // (Ако е зимно време, използвай 7200)
  epochTime += 10800;

  // Записваме го във вградения RTC
  RTCTime timeToSet(epochTime);
  RTC.setTime(timeToSet);
  
  Serial.println("Вграденият RTC е успешно сверен!");
  
  // Изключваме Wi-Fi модула, за да пестим енергия - вече не ни трябва!
  WiFi.disconnect();
}

void loop() {
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  Serial.print("Текущо време: ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  if (currentTime.getMinutes() < 10) Serial.print("0");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  if (currentTime.getSeconds() < 10) Serial.print("0");
  Serial.println(currentTime.getSeconds());
  
  delay(5000);
}