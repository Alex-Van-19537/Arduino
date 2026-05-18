#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // Понякога е .h, провери в библиотеката
unsigned long cTime = 0;
unsigned long sec = 0;
unsigned long min = 0;
unsigned long h = 0;

// SDA (Data) се свързва винаги към А4

// SCL (Clock) се свързва винаги към А5

// Задай адреса на дисплея (обикновено 0x27 или 0x3F) и размера (16 колони, 2 реда)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  lcd.init();          // Инициализация
  lcd.backlight();     // Включване на подсветката
  
  lcd.setCursor(0, 0); // Отиди на 1-ви ред, 1-ва колона
  lcd.print("Running Time: ");
}

void loop() {
  cTime = millis()/1000;
  sec = cTime%60;
  min = (cTime/60)%60;
  h = cTime/3600;


  lcd.setCursor(0, 1);// Преместване на курсора на втория ред
  if ((h/10)==0) {
    lcd.print("0");
  }
  lcd.print(h);
  lcd.print("h:");
  if ((min/10)==0) {
    lcd.print("0");
  }
  lcd.print(min);
  lcd.print("min:");
  if ((sec/10)==0) {
    lcd.print("0");
  }
  lcd.print(sec);
  lcd.print("sec");
}