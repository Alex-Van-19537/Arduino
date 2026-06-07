#ifndef FUNCS_H
#define FUNCS_H
#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "DHT.h"

String readDHT(DHT&);
void disconnectAndSleep(MqttClient& mqttClient, unsigned long timer);
void connectToWifi();
void connectToMQTT(MqttClient&);
void sendToMQTT(MqttClient&, const char*, String);

#endif