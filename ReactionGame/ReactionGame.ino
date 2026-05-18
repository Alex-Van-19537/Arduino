#include <LowPower.h>

const int startButton = 2;
const int greenLed = 3;
const int yellowLed = 4;
const int redLed = 5;
const int p1Led = 6;
const int p2Led = 7;
const int p1Button = 8;
const int p2Button = 9;
const int Beep = 10;
bool redState = false;
bool scoreF = false;

unsigned long previousMillis = 0;
unsigned long waitTime = 0;
unsigned long blinkTime = 0;
unsigned long beepTime = 0;
int p1Score = 0;
int p2Score = 0;

enum State { IDLE, READY, WAIT_GREEN, GO, WINNER, FALSE_START };
State gameState = IDLE;

void wakeUp () {

}

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(p1Led, OUTPUT);
  pinMode(p2Led, OUTPUT);
  pinMode(Beep, OUTPUT);
  pinMode(p1Button, INPUT_PULLUP);
  pinMode(p2Button, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  // Serial.begin(9600);  // За резултат през Serial

}

void loop() {

  unsigned long currentMillis = millis();

  switch (gameState) {

    case IDLE:
      // if (!scoreF) {  // За резултат през Serial
      //   Serial.print("P1 ");
      //   Serial.print(p1Score);
      //   Serial.print(" : ");
      //   Serial.print(p2Score);
      //   Serial.println(" P2");
      //   scoreF = true;
      // }
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, LOW);
      digitalWrite(p1Led, LOW);
      digitalWrite(p2Led, LOW);
      noTone(Beep);
      previousMillis = currentMillis;
      attachInterrupt(0, wakeUp, LOW);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
      detachInterrupt(0);
      if (digitalRead(startButton) == LOW) {
      tone(Beep, 300, 100);
      delay(100);
      tone(Beep, 600, 100);
      delay(100);
      tone(Beep, 900, 100);
      gameState = READY;
      }
      break;

    case READY:
      digitalWrite(redLed, HIGH);
      if (currentMillis - previousMillis > 2000) {
        digitalWrite(yellowLed, HIGH);
        waitTime = random(1000, 8000);
        previousMillis = currentMillis;
        gameState = WAIT_GREEN;
      }
      break;
    
    case WAIT_GREEN:
      if (digitalRead(p1Button) == LOW) {
        digitalWrite(p2Led, HIGH);
        p2Score++;
        previousMillis = currentMillis;
        gameState = FALSE_START;
      }
      else if (digitalRead(p2Button) == LOW) {
        digitalWrite(p1Led, HIGH);
        p1Score++;
        previousMillis = currentMillis;
        gameState = FALSE_START;        
      }
      else {
        if (currentMillis - previousMillis > waitTime) {
          digitalWrite(redLed, LOW);
          digitalWrite(yellowLed, LOW);
          digitalWrite(greenLed, HIGH);
          previousMillis = currentMillis;
          gameState = GO;
        }
      }
      break;
    case GO:
      if (digitalRead(p1Button) == LOW) {
        digitalWrite(p1Led, HIGH);
        p1Score++;
        previousMillis = currentMillis;
        gameState = WINNER;
      }
      else if (digitalRead(p2Button) == LOW) {
        digitalWrite(p2Led, HIGH);
        p2Score++;
        previousMillis = currentMillis;
        gameState = WINNER;
      }
      break;
    
    case WINNER:
      tone(Beep, 1000, 500);
      if (currentMillis - previousMillis > 700) {
        gameState = IDLE;
      }
      scoreF = false;
      break;
    
    case FALSE_START:
      digitalWrite(yellowLed, LOW);
      if (currentMillis - blinkTime >= 250) {
        blinkTime = currentMillis;
        redState = !redState;
        digitalWrite(redLed, redState);
        if(redState) {
          tone(Beep, 200, 100);
        }
      }   
      if (currentMillis - previousMillis > 2000) {
        gameState = IDLE;
        scoreF = false;
        break;
      }
  }
}
