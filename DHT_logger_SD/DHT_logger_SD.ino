#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include "RTC.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.;
float h = 0.;
unsigned long currentMillis;
unsigned long previousMillis;
const long TIMER = 1200000;
const String LOGFILE = "log.csv";
const int CHIP_SELECT = 10;
File file;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  initSD();
  dht.begin();
  Serial.end();
  logData();
}

void loop(){
  currentMillis = millis();
  if(currentMillis - previousMillis >= TIMER) {
    logData();
    previousMillis = currentMillis;
  }
}

void initSD(){
  Serial.print("Initializing SD card...");

  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("initialization failed.");
    while (1);
  }
  Serial.println("initialization done.");
}

void logData(){
  file = SD.open(LOGFILE, FILE_WRITE);
  if(file){
    RTCTime currentTime;
    RTC.getTime(currentTime);
    for(int i=0; i<5; i++){
      h += dht.readHumidity();
      t += dht.readTemperature();
      delay(50);
    }
    h /= 5;
    t /= 5;
    String dataStr = String(currentTime.getHour()) + ":" + String(currentTime.getMinutes()) + ", " + t + ", " + h;
    file.println(dataStr);
    h=0.;
    t=0.;
    file.close();
  }
  else while(1);
}

