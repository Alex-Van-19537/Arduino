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

const int latchPin = 10;  //ST_CP (pin 12 595)

const int clockPin = 11;  //SH_CP (pin 11 595)

const int dataPin = 12;  //DS (pin 14 595)


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  //--- Binary Counter For 5 LEDs ---
  for(int i = 0; i<32; i++){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, i);
    digitalWrite(latchPin, HIGH);
    delay(400);
  }


  // //--- Knight Rider ---
  // int shiftedLed = 1;
  // shiftedLed <<= 1;

  // for(int i = 0; i < 4; i++) {
  //   digitalWrite(latchPin, LOW);
  //   shiftOut(dataPin, clockPin, MSBFIRST, shiftedLed);
  //   digitalWrite(latchPin, HIGH);
  //   delay(100);
  //   shiftedLed <<= 1;
  // }

  // shiftedLed >>= 2;

  // for(int i = 0; i < 4; i++) {
  //   digitalWrite(latchPin, LOW);
  //   shiftOut(dataPin, clockPin, MSBFIRST, shiftedLed);
  //   digitalWrite(latchPin, HIGH);
  //   delay(100);
  //   shiftedLed >>= 1;
  // }
}
