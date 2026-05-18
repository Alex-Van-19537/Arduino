#include <Stepper.h>

// Брой стъпки за един пълен оборот (за 28BYJ-48 обикновено е 2038 или 2048)
const int stepsPerRevolution = 2048;

// Инициализираме библиотеката на пинове 8, 10, 9, 11
// РЕДЪТ Е ВАЖЕН: IN1, IN3, IN2, IN4
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Задаваме скорост (обороти в минута)
  // Не слагай твърде висока скорост (над 15-20), защото моторът ще спре
  myStepper.setSpeed(10);
  
  Serial.begin(9600);
}

void loop() {
  myStepper.step(stepsPerRevolution);
  delay(1000);
  myStepper.step(-stepsPerRevolution);
  delay(1000);  // Един пълен оборо
}