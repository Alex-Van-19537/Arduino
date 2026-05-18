const int ledPin = 10;
unsigned long previousMillis = 0;
const long blinkInterval = 500;
int ledState = LOW;

void blink(int);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(13, LOW);
  // put your setup code here, to run once:

}

void loop() {
  blink(ledPin);
  // put your main code here, to run repeatedly:

}

void blink(int pin) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    digitalWrite(pin, (ledState = ledState==LOW?HIGH:LOW));
  }
}
