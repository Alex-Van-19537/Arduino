const int ldrPin = A0;
const int ledPin = 3;
const int clapPin = 6;
int brightness = 0;
bool isOn = false;
bool clap = false;
unsigned long lastClapTime;
enum Direction {ON, OFF};

void fadeLed (Direction d) {
  switch (d){
    case ON:
      if (isOn) break;
      for (int i=0; i<=100; i++){
        analogWrite(ledPin, map(i, 0, 100, 0, 255));
        delay(5);
      }
      isOn = true;
      break;
    case OFF:
      if(!isOn) break;
      for (int i=0; i<=100; i++){
        analogWrite(ledPin, map(i, 0, 100, 255, 0));
        delay(5);         
      }
      isOn = false;
      break;
  }
}

void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(clapPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(clapPin) && millis() - lastClapTime >= 500) {
    clap=!clap;
    lastClapTime = millis();
  } 
  brightness = map(analogRead(ldrPin), 0, 1023, 0, 100);
  Serial.println(clap);
  if (brightness<=10) {
    clap?fadeLed(ON):fadeLed(OFF);
  }
  else {
    fadeLed(OFF);
    clap = false;
  }
}
