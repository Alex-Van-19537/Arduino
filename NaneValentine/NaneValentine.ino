#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "Battery_Animation.h"
ArduinoLEDMatrix matrix;
unsigned long prevMillis = 0;
bool animeF = true;

void setup() {
  matrix.begin();
}

void loop() {
  matrix.beginDraw();
  matrix.textScrollSpeed(100);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFF);
  matrix.println("    Will you be my valentine?");
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  matrix.loadSequence(LEDMATRIX_ANIMATION_HEARTBEAT);
  matrix.play();
  delay(3000);

  matrix.loadFrame(LEDMATRIX_HEART_BIG);
  delay(3000);


  // put your main code here, to run repeatedly:

}
