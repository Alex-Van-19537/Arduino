//        +---+--+---+
//  Q1   1 |   U   | 16  Vcc
//  Q2   2 |       | 15  Q0
//  Q3   3 |       | 14  DS (Data Serial In) Accepts bits from Arduino
//  Q4   4 |       | 13  OE (Output Enable) LOW - Output ON / HIGH - Output OFF
//  Q5   5 |       | 12  ST_CP (Latch) -> lock(HIGH)/unlock(LOW) Output
//  Q6   6 |       | 11  SH_CP (Clock) -> every signal moves to the next Output position
//  Q7   7 |       | 10  MR (Master Reset) reset if LOW
//  GND  8 |       | 9   Q7' for more 595 (connect to DS of the next 595 and Clock and Latch are the same)
//        +---+--+---+


// 595 pin 16 -> V5+
// 595 pin 10 -> V5+
// 595 pin 8 -> GND
// 595 pin 13 -> GND

// разход на ток при сън -> 7.18 mA
// разход на ток при IDLE -> 66 mA         # разходът е мерен като реален -
// разход на ток MAX (READY) -> 193 mA     # преди МТ модула на захранването!

#include <LowPower.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int startButton = 2;
const int latchPin = 11;  //ST_CP (pin 12 595)
const int clockPin = 10;  //SH_CP (pin 11 595)
const int dataPin = 12;  //DS (pin 14 595)
const int p1Button = 9;
const int p2Button = 8;
const int resetScore = 5;
const int lcdPowerPin = 4; //connected to (1K omh) -> BASE of NPN transistor between GND and lcd GND
const int Beep = 3;
const unsigned long SLEEP_WARNING_TIME = 20000;
const unsigned long SLEEP_TIME = 25000;
bool scoreF = false;
bool sleepF = false;
unsigned long previousMillis = 0;
unsigned long waitTime = 0;
unsigned long blinkTime = 0;
unsigned long beepTime = 0;
unsigned long timer = 0;
unsigned long reactionTime = 0;
int p1Score = 0;
int p2Score = 0;

enum State { 
  IDLE,
  READY,
  WAIT_GREEN,
  GO,
  WINNER,
  FALSE_START
};

State gameState = IDLE;

enum LedState {
  P1WIN = 0b10000000,
  P2WIN = 0b00000001,
  RED = 0b01000000,
  YELLOW = 0b01010000,
  GREEN = 0b00000100,
  ALL_OFF = 0b00000000,
  FSP1 = 0b01000001,
  FSP2 = 0b11000000
};

LiquidCrystal_I2C lcd(0x27, 16, 2);

LedState falseStartEffect = RED;
LedState winner;

void write595(LedState);
void wakeUp () {}

void setup() {
  pinMode(lcdPowerPin, OUTPUT);
  digitalWrite(lcdPowerPin, HIGH);
  lcd.init();
  lcd.backlight();
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(Beep, OUTPUT);
  pinMode(p1Button, INPUT_PULLUP);
  pinMode(p2Button, INPUT_PULLUP);
  pinMode(resetScore, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  randomSeed(analogRead(A0));
}

void loop() {

  unsigned long currentMillis = millis();

  switch (gameState) {

    case IDLE:
      winner = ALL_OFF;
      write595(ALL_OFF);
      noTone(Beep);
      timer = 0;
      reactionTime = 0;
      if (digitalRead(startButton) == LOW) {
        previousMillis = currentMillis;
        tone(Beep, 300, 100);
        delay(100);
        tone(Beep, 600, 100);
        delay(100);
        tone(Beep, 900, 100);
        waitTime = random(1000, 8000);
        gameState = READY;
        sleepF = false;
        scoreF = true;
        lcd.clear();
        lcd.print("READY...");
      }
      if ((currentMillis - previousMillis >= SLEEP_WARNING_TIME) && (currentMillis - previousMillis < SLEEP_TIME) && !sleepF){
        lcd.clear();
        lcd.print("ENTERING");
        lcd.setCursor(0, 1);
        lcd.print("SLEEPING MODE...");
        sleepF = true;
      }
      if (currentMillis - previousMillis >= SLEEP_TIME) {
        attachInterrupt(0, wakeUp, LOW);
        lcd.noBacklight();
        lcd.noDisplay();
        Wire.end();
        pinMode(A4, INPUT);
        pinMode(A5, INPUT);
        digitalWrite(lcdPowerPin, LOW);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        detachInterrupt(0);
        digitalWrite(lcdPowerPin, HIGH);
        delay(150);
        Wire.begin();
        lcd.init();
        lcd.backlight();
        lcd.display();
        scoreF = false;
        sleepF = false;
        previousMillis = currentMillis;
      }
      if (digitalRead(resetScore) == LOW && !(p1Score == 0 && p2Score == 0) && (currentMillis - previousMillis < SLEEP_WARNING_TIME)) {
        p1Score = 0;
        p2Score = 0;
        scoreF = false;
        previousMillis = currentMillis;
      }

      if (!scoreF && (currentMillis - previousMillis < SLEEP_WARNING_TIME)) {
        lcd.clear();
        lcd.print("PLAYER 1: ");
        lcd.print(p1Score);
        lcd.setCursor(0, 1);
        lcd.print("PLAYER 2: ");
        lcd.print(p2Score);
        scoreF = true;
      }

      if((digitalRead(p1Button) == LOW ||
          digitalRead(p2Button) == LOW ||
          digitalRead(resetScore) == LOW)
          && sleepF) {
        previousMillis = currentMillis;
        scoreF = false;
        sleepF = false;
      }

      break;
    
    case READY:
      write595(RED);
      if (currentMillis - previousMillis > waitTime) {
        lcd.setCursor(0, 0);
        lcd.print("STEADY...");
        write595(YELLOW);
        previousMillis = currentMillis;
        waitTime = random(1000, 8000);
        gameState = WAIT_GREEN;
      }
      break;

    case WAIT_GREEN:
      if (digitalRead(p1Button) == LOW) {
        winner = P2WIN;
        p2Score++;
        previousMillis = currentMillis;
        gameState = FALSE_START;
        lcd.clear();
      }
      else if (digitalRead(p2Button) == LOW) {
        winner = P1WIN;
        p1Score++;
        previousMillis = currentMillis;
        gameState = FALSE_START; 
        lcd.clear();       
      }
      else {
        if (currentMillis - previousMillis > waitTime) {
          lcd.setCursor(0, 0);
          lcd.print("GO, GO, GO!!!");
          write595(GREEN);
          timer = millis();
          previousMillis = currentMillis;
          gameState = GO;
        }
      }
      break;

    case GO:
      if (digitalRead(p1Button) == LOW) {
        reactionTime = millis() - timer;
        winner = P1WIN;
        write595(winner);
        lcd.clear();
        lcd.print("PLAYER 1 WINS!!!");
        lcd.setCursor(0, 1);
        lcd.print("Reaction: ");
        lcd.print(reactionTime);
        lcd.print(" ms");
        p1Score++;
        previousMillis = currentMillis;
        gameState = WINNER;
      }
      else if (digitalRead(p2Button) == LOW) {
        reactionTime = millis() - timer;
        winner = P2WIN;
        write595(winner);
        lcd.clear();
        lcd.print("PLAYER 2 WINS!!!");
        lcd.setCursor(0, 1);
        lcd.print("Reaction: ");
        lcd.print(reactionTime);
        lcd.print("ms");
        p2Score++;
        previousMillis = currentMillis;
        gameState = WINNER;
      }
      break;

    case WINNER:
    if(currentMillis - previousMillis <= 300)
      tone(Beep, 1000, 300);
      if (currentMillis - previousMillis > 5000) {
        gameState = IDLE;
        previousMillis = currentMillis;
      }
      scoreF = false;
      break;

    case FALSE_START:
      lcd.print("PLAYER ");
      lcd.print(winner==P1WIN?"2":"1");
      lcd.setCursor(0, 1);
      lcd.print("FALSE START  !!!");
      if (currentMillis - blinkTime >= 250) {
        blinkTime = currentMillis;
        if (winner == P1WIN) {
          falseStartEffect = (falseStartEffect==FSP2?ALL_OFF:FSP2);
        }
        if (winner == P2WIN) {
          falseStartEffect = (falseStartEffect==FSP1?ALL_OFF:FSP1);
        }
        write595(falseStartEffect);
        if(falseStartEffect!=ALL_OFF) {
          tone(Beep, 200, 100);
        }
      }   
      if (currentMillis - previousMillis > 3000) {
        gameState = IDLE;
        scoreF = false;
        previousMillis = currentMillis;
        break;
      }
  }
}

void write595(LedState state) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (byte)state);
  digitalWrite(latchPin, HIGH);
}
