#include <CoDrone.h>

unsigned long SetTime;
unsigned long PulseWidth;

void setup() {
}

void loop() {
  int LeftJoystick = CoDrone.AnalogScaleChange(analogRead(23)); 
  PulseWidth = (LeftJoystick + 100)*50;

  if (micros() - SetTime < PulseWidth) {
    digitalWrite(13, HIGH); 
  }
  else if (micros() - SetTime < 20000) {
    digitalWrite(13, LOW);
  } 
  else {
    SetTime = micros(); 
  } 
}







