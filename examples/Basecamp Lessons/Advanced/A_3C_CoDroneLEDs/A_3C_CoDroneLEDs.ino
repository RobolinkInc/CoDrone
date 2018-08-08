/*------------------------------------------------------------------
// This is the sample code for Lesson 3C:CoDroneLEDs
// https://basecamp.robolink.com/cwists/preview/831x
-------------------------------------------------------------------*/

#include <CoDrone.h>

unsigned long StartTime;
int StartFlag;

void setup() {
  CoDrone.begin(115200);
  CoDrone.pair();               // Pair with one the last drone this remote paired with
  CoDrone.setArmMode(LED_DIMMING); // Change Arm LED mode to hold LED.
  CoDrone.takeoff();          
}

void loop() {
  byte bt1 = digitalRead(11);

  if (bt1) {
    StartFlag = 1;
    StartTime = millis();
  }

  if (StartFlag == 1) {
    if (millis() - StartTime < 1000) { 
      CoDrone.setEyeRGB(255,255,255);  // Change eye LED color to RGB 255, 255, 255(white) 
      CoDrone.setThrottle(50);
      CoDrone.move();
    }
    else if ( millis() - StartTime < 3000) {
      CoDrone.setEyeRGB(0,0,255);  // Change eye LED color to RGB 0, 0, 255(blue) 
      CoDrone.setThrottle(0);
      CoDrone.setPitch(50);
      CoDrone.move(); 
    }
    else { 
      CoDrone.land();
      StartFlag = 0; 
    }
  }
}