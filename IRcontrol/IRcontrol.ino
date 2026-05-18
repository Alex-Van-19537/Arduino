#include <IRremote.hpp>

const int RCV_PIN = 4;
const int LED_PIN = 6;
int command;
int ledBrightess = 6;
bool ledState = false;
unsigned long last = 0;

const int BTN_POWER = 67;
const int BTN_PLUS  = 21;
const int BTN_MINUS = 7;

void setup() {
  Serial.begin(9600);
  pinMode(RCV_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  IrReceiver.begin(RCV_PIN, ENABLE_LED_FEEDBACK);

}

void loop() {
  if (IrReceiver.decode() && millis() - last > 50) {
    // Проверява дали сигналът не е грешка или "repeat" (задържан бутон)
    if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
      command = IrReceiver.decodedIRData.command;
      Serial.println(command);
      if (IrReceiver.decodedIRData.decodedRawData != 0) {
        switch (command) {
          case BTN_POWER:
            ledState = !ledState;
            analogWrite(LED_PIN, ledState?map(ledBrightess, 0, 6, 0, 255):0);
            break;
          case BTN_PLUS:
            if (ledBrightess < 6 && ledState) {
              ledBrightess++;
              analogWrite(LED_PIN, map(ledBrightess, 0, 6, 0, 255));
            }
            break;
          case BTN_MINUS:
            if (ledBrightess > 1 && ledState) {
              ledBrightess--;
              analogWrite(LED_PIN, map(ledBrightess, 0, 6, 0, 255));
            }
            break;
        }
      }

      Serial.print("Led state: ");
      Serial.println(ledState);

      Serial.print("Led Brightness: ");
      Serial.println(ledBrightess);
      last = millis();
    }
    
    
    IrReceiver.resume(); // Подготвя за следващия сигнал
  }

}
