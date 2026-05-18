unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long debounce = 0;
unsigned long startTime = 0;
const int led = 13;
const int onLed = 6;
const int button = 11;
const int pot = A0;
const int SYS_TIMEOUT = 3600000;
bool buttonF = false;
int buttonState = 0;
int reading = 0;
int interval = 0;
int ledState = LOW;


int getInterval(int);
int getButton(int, int);


void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(pot, INPUT);
  pinMode(onLed, OUTPUT);
}


void loop() {
  currentMillis = millis();

  interval = getInterval(analogRead(pot));

  reading = getButton(digitalRead(button), buttonState);
  if (reading == LOW && buttonState == HIGH) {
    buttonF = !buttonF;
    startTime = currentMillis;
  }
  buttonState = reading;

  if (currentMillis - startTime >= SYS_TIMEOUT && buttonF) buttonF = !buttonF;

  digitalWrite(onLed, buttonF ? HIGH : LOW);

  if (currentMillis - previousMillis >= interval && buttonF) {
    ledState = !ledState;
    digitalWrite(led, ledState);
    Serial.println(ledState);
    previousMillis = currentMillis;
  }

  else if (!buttonF) {
    ledState = LOW;
    digitalWrite(led, ledState);
    previousMillis = currentMillis - interval;
  }
}




int getInterval(int val) {
  val = constrain(val, 0, 1023);
  return map(val, 0, 1023, 100, 3000);
}


int getButton(int state, int prevState) {
  if (currentMillis - debounce >= 50) {
    debounce = currentMillis;
    return state;
  }
  return prevState;
}
