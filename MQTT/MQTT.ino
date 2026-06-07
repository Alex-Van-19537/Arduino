#include "funcs.h"
#define DHTPIN 8
#define DHTTYPE DHT22

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

const char topic[]  = "arduino/sensor";
const long interval = 600000;
const int sigPin = 13;

void setup() {
  // Serial.begin(9600);
  // while (!Serial){};
  pinMode(sigPin, OUTPUT);
  connectToWifi();
  dht.begin();
  // Serial.println("DHT init success!");
}

void loop() {
  digitalWrite(sigPin, 1);
  connectToMQTT(mqttClient);
  String reading = readDHT(dht);
  sendToMQTT(mqttClient, topic, reading);
  digitalWrite(sigPin, 0);
  disconnectAndSleep(mqttClient, interval);
}
