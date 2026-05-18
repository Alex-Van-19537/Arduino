#include "Arduino.h"
#include "funcs.h"
#include "arduino_secrets.h"

String readDHT(DHT &dht){
  // Премахнати dht.begin() и delay() - сензорът вече е готов за четене!
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  // Проверка за грешки при четенето
  if (isnan(h) || isnan(t)) {
    t = 0;
    h = 0;
  }

  // Създаване на JSON структурата
  String jsonPayload = "{";
  jsonPayload += "\"temperature\": ";
  jsonPayload += t;
  jsonPayload += ", \"humidity\": ";
  jsonPayload += h;
  jsonPayload += "}";
  
  return jsonPayload;
}

void connectToWifi(){
  char ssid[] = SECRET_SSID;
  char pass[] = SECRET_PASS;
  int retries = 0; // Брояч на опитите
  
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  
  // Опитваме максимум 5 пъти (общо 25 секунди)
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
    retries++;
    if (retries >= 5) {
      Serial.println("\nWiFi connection failed! Aborting.");
      return; // Прекъсваме опита, за да спасим батерията
    }
  }

  Serial.println("\nYou're connected to the network");
}

void connectToMQTT(MqttClient &mqttClient){
  const char broker[] = "192.168.0.188";
  int        port     = 1883;
  int        retries  = 0; // Брояч на опитите

  mqttClient.setId("Arduino");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  // Опитваме максимум 3 пъти (общо 15 секунди)
  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(5000);
    retries++;
    if (retries >= 5) {
      Serial.println("\nMQTT connection aborted.");
      return; // Прекъсваме опита
    }
  }

  Serial.println("\nYou're connected to the MQTT broker!");
}

void sendToMQTT(MqttClient& mqttClient, const char* topic, String message){
  // Пращаме само ако имаме реална връзка
  if (mqttClient.connected()) {
    mqttClient.beginMessage(topic);
    mqttClient.print(message);
    mqttClient.endMessage();
  } else {
    Serial.println("Skipping send: Not connected to MQTT.");
  }
}