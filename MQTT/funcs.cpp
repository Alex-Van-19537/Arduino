#include "Arduino.h"
#include "funcs.h"
#include "arduino_secrets.h"

String readDHT(DHT &dht){
    float h = dht.readHumidity();
    float t = dht.readTemperature(); 

    // Проверка за грешки при четенето
    if (isnan(h) || isnan(t)) {
      t = 0;
      h = 0;
    }

    // Създаване на JSON структурата
    // Резултатът ще изглежда така: {"temperature": 24.50, "humidity": 45.20}
    String jsonPayload = "{";
    jsonPayload += "\"temperature\": ";
    jsonPayload += t;
    jsonPayload += ", \"humidity\": ";
    jsonPayload += h;
    jsonPayload += "}";
    // Serial.println(jsonPayload);
    return jsonPayload;
}

void connectToWifi(){
  char ssid[] = SECRET_SSID;
  char pass[] = SECRET_PASS;
    // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectToMQTT(MqttClient &mqttClient){

  const char broker[] = "192.168.0.188";
  int        port     = 1883;

  mqttClient.setId("Arduino");
  // mqttClient.setUsernamePassword(USERNAME, PASSWORD);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    Serial.println("Trying again...");
    delay(5000);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void sendToMQTT(MqttClient& mqttClient, const char* topic, String message){
    // Serial.println(topic);
    mqttClient.beginMessage(topic);
    // Serial.println(message);
    mqttClient.print(message);
    mqttClient.endMessage();
    // Serial.println("Sent!");
}