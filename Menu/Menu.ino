const int potPin = A1;
const int beepPin = 3;
int potValue = 0;
int menuCount = 4;
int currentMenu = 0;
int prevMenu = -1;

void clearLine () {
  Serial.print("\n");
}

int readStablePot() {
  long sum = 0;
  const int samples = 5;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(potPin);
    delay(2);
  }
  return sum/samples;
  
}



void setup() {
  Serial.begin(9600);
  Serial.println("Current menu:");
  // put your setup code here, to run once:

}

void loop() {
  potValue = readStablePot();
  if (potValue<=240) currentMenu = 0;

  else if (potValue >=260 && potValue<=500) currentMenu = 1;

  else if (potValue >=520 && potValue<=760) currentMenu = 2;

  else if (potValue >=780) currentMenu = 3;

  else currentMenu = prevMenu;

  prevMenu = currentMenu;
  
    switch(currentMenu) {
      case 0:
        Serial.println("General");
        break;
      case 1:
        Serial.println("Sound");
        break;
      case 2:
        Serial.println("Display");
        break;
      case 3:
        Serial.println("Bluetooth");
        break;
    }
}
