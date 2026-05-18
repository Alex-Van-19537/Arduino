#include "funcs.h"
#define DHTPIN 8    // Пинът, към който е свързан сензора
#define DHTTYPE DHT22   // Дефинираме типа сензор

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

const char topic[]  = "arduino/sensor";
const long interval = 30000;
unsigned long previousMillis = 0;
const int sigPin = 13;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(sigPin, OUTPUT);
  while (!Serial){};
  dht.begin();
  Serial.println("DHT init success!");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    digitalWrite(sigPin, 1);
    connectToWifi();
    connectToMQTT(mqttClient);

    String reading = readDHT(dht);
    Serial.println(reading);
    sendToMQTT(mqttClient, topic, reading);
    digitalWrite(sigPin, 0);
    WiFi.disconnect();
    WiFi.end();
    previousMillis = currentMillis;
  }
}
