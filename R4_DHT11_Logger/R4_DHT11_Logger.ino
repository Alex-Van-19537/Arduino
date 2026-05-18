#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
ArduinoLEDMatrix matrix;

// Дефинираме празен екран (3 нули), за да го ползваме за изчистване
const uint32_t blackFrame[] = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  dht.begin();
  matrix.begin();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t)) t = 0;
  if (isnan(h)) h = 0;

  // ВАЖНО: Само това трябва да отива към Serial, за да не се бърка плотера
  Serial.print(t); 
  Serial.print(","); 
  Serial.println(h); 

  // Всички други Serial.print ("Temp:", "Humidity:") ги изтрий или сложи на коментар //
  
  // Логиката за екрана си остава същата
  if (t == 0 && h == 0) {
    drawStaticText("Er");
  } 
  else {
    matrix.loadFrame(blackFrame); 
    delay(200);
    drawStaticText(String((int)t));
    delay(2000); 
    matrix.loadFrame(blackFrame); 
    delay(200);
    drawStaticText(String((int)h));
    delay(2000); 
  }
}

void drawStaticText(String text) {
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_5x7);
  matrix.beginText(1, 1, 0xFFFFFFFF);
  matrix.print(text);
  matrix.endText();
  matrix.endDraw();
}