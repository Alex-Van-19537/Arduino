#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

unsigned long currentMillis = 0;
unsigned long lastReadMillis = 0;   
unsigned long lastShowMillis = 0;   

const unsigned long seedPeriod = 10;      // Четем на всеки 10ms
const unsigned long showReading = 10000;  // Показваме на всеки 10s

ArduinoLEDMatrix matrix;
const int batPin = A0;

int percent = 0;
long sum = 0;       // Използваме long, за да не препълним буфера
int count = 0;      // Брояч колко пъти сме измерили реално

void setup() {
  Serial.begin(115200);
  matrix.begin(); 
  analogReadResolution(14); 

  // --- МОМЕНТАЛЕН СТАРТ ---
  // Правим едно бързо измерване и рисуване веднага, 
  // за да не стои екрана тъмен 10 секунди.
  // Serial.println("Light up all pixels...");
  // matrix.beginDraw();
  // matrix.stroke(255, 255, 255); // RGB бяло
  // matrix.rect(0, 0, 12, 8);     // Плътен правоъгълник
  // matrix.endDraw();
  
  // delay(2000); // Чакаме 2 секунди
}

void loop() {
  currentMillis = millis();

  // --- ТАЙМЕР 1: Четене и сумиране ---
  if(currentMillis - lastReadMillis >= seedPeriod) {
    lastReadMillis = currentMillis; 
    
    int raw = analogRead(batPin);
    int currentVal = 0;

    if (raw > 9830) {
        currentVal = map(raw, 9830, 13763, 0, 100);
    } 
    
    // Ограничения преди сумирането
    if (currentVal > 100) currentVal = 100;
    if (currentVal < 0) currentVal = 0;

    sum += currentVal;
    count++; // Увеличаваме брояча на измерванията
  }

  // --- ТАЙМЕР 2: Изчисляване и Рисуване ---
  if(currentMillis - lastShowMillis >= showReading) {
    lastShowMillis = currentMillis; 
    
    // Изчисляваме средното аритметично само ако имаме данни
    if (count > 0) {
      percent = sum / count; 
    } else {
      percent = 0;
    }

    // Нулираме за следващия цикъл
    sum = 0;
    count = 0;

    // Извикваме функцията за рисуване
    drawBattery(percent);
  }
}

// // --- ФУНКЦИЯ ЗА РИСУВАНЕ ---
// // Изнесохме я отвън, за да може да се ползва и в setup(), и в loop()
// void drawBattery(int val) {
//   matrix.beginDraw();
//   matrix.clear(); // Изчистваме стария капацитет
  
//   matrix.stroke(0xFFFFFFFF);
//   matrix.textFont(Font_5x7); // Използвай по-малък шрифт, 5x7 е голям за 12x8 матрица
  
//   // Позиционираме курсора ръчно
//   // За 1-2 цифри: x=1, y=1 е добра стартова точка
//   matrix.beginText(1, 1, 0xFFFFFFFF); 
  
//   if (val >= 100) {
//     matrix.print("F");
//   } else {
//     matrix.print(String(val));
//   }
  
//   matrix.endText(); // Важно: приключване на текстовия режим
//   matrix.endDraw();

//   Serial.print("Battery: ");
//   Serial.println(val);
// }

void drawBattery(int val) {
    matrix.beginDraw();
    matrix.clear();
    matrix.textFont(Font_5x7);
    
    if (val >= 100) {
      matrix.beginText(0, 1, 0xFFFFFFFF);
      matrix.print("F"); 
    } else {
      int xPos = 1; 
      if (val < 10) xPos = 4; // Центриране за една цифра
      
      matrix.beginText(xPos, 1, 0xFFFFFFFF); 
      matrix.print(val);
    }

    matrix.endText();
    matrix.endDraw();

    Serial.print("Battery: ");
    Serial.println(String(val));
}