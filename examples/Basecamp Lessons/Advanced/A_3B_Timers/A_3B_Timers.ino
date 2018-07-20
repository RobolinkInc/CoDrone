  /*------------------------------------------------------------------
// This is the sample code for Lesson 3B:Timers
// https://basecamp.robolink.com/cwists/preview/805x
-------------------------------------------------------------------*/

#include <CoDrone.h>

unsigned long StartTime;
int StartFlag;
 
void setup() {
  CoDrone.begin(115200);
  CoDrone.pair();   // Pair with one the last drone this remote paired with

  CoDrone.takeoff();  //take off and delay 3 seconds 
}

void loop() {
  byte bt1 = digitalRead(11);
  byte bt4 = digitalRead(14);
  byte bt8 = digitalRead(18);

  //button trigger timmer
  if (bt1) {
    StartTime = millis();
    StartFlag = 1;
  }
  //timmer make go up-> forward-> right-> landing
  if (StartFlag == 1) {
    if (millis() - StartTime < 2000) {
      CoDrone.setThrottle(30);
      CoDrone.move();
    }
      else if (millis() - StartTime < 5000) {
        CoDrone.setPitch(70);
        CoDrone.setThrottle(0);
        CoDrone.move();
    }
      else if (millis() - StartTime < 7000){
        CoDrone.setRoll(40);
        CoDrone.setPitch(0);
        CoDrone.move();
      }
    else {
      CoDrone.land();
      StartFlag = 0;
    }
  }
  
  if (bt8) {
     CoDrone.emergencyStop();
     StartFlag = 0;
}
