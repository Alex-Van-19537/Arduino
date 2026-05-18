const int sD0 = 10;
const int sD1 = 9;
const int sD2 = 8;
const int sD3 = 11;
const int VT = 12;
const int S = 7;
bool sF = false;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const int DEBOUNCE = 200;


void setup() {

  Serial.begin(9600);

  pinMode(VT, INPUT);
  pinMode(sD0, INPUT);
  pinMode(sD1, INPUT);
  pinMode(sD2, INPUT);
  pinMode(sD3, INPUT);
  pinMode(S, OUTPUT);
}


void loop() {
  currentMillis = millis();
  if(digitalRead(VT)) {
    if(currentMillis - previousMillis >= DEBOUNCE) {  

      Serial.print("Pressed button: ");
      sF = !sF;
      digitalWrite(S, sF);

      if(digitalRead(sD0)) Serial.println("A\n");
      if(digitalRead(sD1)) Serial.println("B\n");
      if(digitalRead(sD2)) Serial.println("C\n");
      if(digitalRead(sD3)) Serial.println("D\n");
    }
    previousMillis = currentMillis;
  }
}