#include <LiquidCrystal.h>

// Създаваме обект на LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int cTime;
int sec;
int min;
int h;

void setup() {
  lcd.begin(16, 2); // 16 колони, 2 реда
  lcd.print("Running for: ");
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
  lcd.print(":");
  if ((min/10)==0) {
    lcd.print("0");
  }
  lcd.print(min);
  lcd.print(":");
  if ((sec/10)==0) {
    lcd.print("0");
  }
  lcd.print(sec);
}