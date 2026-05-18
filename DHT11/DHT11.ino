#include "DHT.h"

#define DHTPIN 2     // Пинът, към който е свързан сензора
#define DHTTYPE DHT11   // Дефинираме типа сензор

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 test!");

  dht.begin();
}

void loop() {
  // Изчакай малко между измерванията
  delay(2000);

  // Четене на влажност и температура
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // По подразбиране е в Целзий

  // Проверка дали четенето е успешно
  if (isnan(h) || isnan(t)) {
    Serial.println("Грешка при четене от DHT сензора!");
    return;
  }

  Serial.print("Влажност: ");
  Serial.print(h);
  Serial.print("%  |  ");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println("°C");
}