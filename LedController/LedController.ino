int potPin = A0;
int ledPin = 3;
int val;
int brightnes = 0;
int brightnesP = 0;
unsigned long lastPrint= 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  val = analogRead(potPin);
  brightnes = map(val,0,1023,0,255);
  brightnesP = map(brightnes, 0, 255, 0 ,100);
  analogWrite(ledPin, brightnes);

  if (millis() - lastPrint >= 500) {
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.print("Led brightnes: ");
    Serial.print(brightnesP);
    Serial.println("%");
    lastPrint = millis();
  }
}
