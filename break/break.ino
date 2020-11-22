#include <Joystick.h>

// Create the Joystick

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 0, 0,
  false, false, false, true, false, false,
  false, false, false, false, false);

void setup() {
  // Initialize Joystick Library
  Joystick.setRxAxisRange(0, 1777);
  Joystick.begin(false);
}

void loop() {
  int16_t val = analogRead(0);
  Serial.println(val);
  Joystick.setRxAxis(val);
  Joystick.sendState();
  delay(10);
}