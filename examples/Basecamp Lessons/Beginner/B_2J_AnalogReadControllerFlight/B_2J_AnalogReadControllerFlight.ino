/*------------------------------------------------------------------
This is the sample code for Lesson 2J: Analog Read and Controller Flight
https://basecamp.robolink.com/cwists/preview/774x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup() {
    CoDrone.begin(115200);
    CoDrone.pair();
}

void loop() {
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);

  if (PAIRING == true) {

    CoDrone.setRoll(-1 * CoDrone.AnalogScaleChange(analogRead(24))); // left and right on the right joystick
    CoDrone.setPitch(CoDrone.AnalogScaleChange(analogRead(25)));     // up and down on the right joystick
    CoDrone.setYaw(-1 * CoDrone.AnalogScaleChange(analogRead(22)));  // left and right on the left joystick
    CoDrone.setThrottle(CoDrone.AnalogScaleChange(analogRead(23)));  // up and down on the left joystick 

    CoDrone.move();
  }

  if(bt1 && !bt4 && !bt8){
    CoDrone.emergencyStop();
  }
}