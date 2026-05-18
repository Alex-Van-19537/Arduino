#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "RTC.h"
ArduinoLEDMatrix matrix;

void setup() {
  matrix.begin();
  RTC.begin();
  Serial.begin(9600);
  // matrix.loadSequence(BATTERY_ANIMATION);
  // matrix.play();
  // delay(3000);
  // matrix.loadSequence(LEDMATRIX_ANIMATION_INFINITY_LOOP_LOADER);
  // matrix.play();
  // put your setup code here, to run once:

}

void loop() {
  RTCTime currenttime;
  RTC.getTime(currenttime);

  // Serial.print("Current time: ");
  // /* DATE */
  // Serial.print(currenttime.getDayOfMonth());
  // Serial.print("/");
  // Serial.print(Month2int(currenttime.getMonth()));
  // Serial.print("/");
  // Serial.print(currenttime.getYear());
  // Serial.print(" - ");

  // /* ORE:MINUTI:SECONDI */
  // Serial.print(currenttime.getHour());
  // Serial.print(":");
  // Serial.print(currenttime.getMinutes());
  // Serial.print(":");
  // Serial.println(currenttime.getSeconds());

  String date = "    "
                + String(currenttime.getDayOfMonth()) 
                + "." 
                + String((int)currenttime.getMonth() + 1)
                + "."
                + String(currenttime.getYear())
                + "    "
                + String(currenttime.getHour()) 
                + ":" 
                + String(currenttime.getMinutes() + 1)
                + ":"
                + String(currenttime.getSeconds());

  matrix.beginDraw();
  matrix.textScrollSpeed(70);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFF);
  matrix.println(date);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  // matrix.loadSequence(LEDMATRIX_ANIMATION_HEARTBEAT);
  // matrix.play();
  // delay(3000);

  // matrix.loadFrame(LEDMATRIX_HEART_BIG);
  // delay(3000);


  // put your main code here, to run repeatedly:

}
