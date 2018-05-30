// This is the sample code for Lesson 3B:Timers
// https://basecamp.robolink.com/cwists/preview/805x

#include <CoDrone.h>

unsigned long StartTime;
int StartFlag;
 
void setup() {
  CoDrone.begin(115200);
  CoDrone.AutoConnect(NearbyDrone);

  CoDrone.FlightEvent(TakeOff);   
  delay(2000);
}

void loop() {
  byte bt1 = digitalRead(11);
  byte bt4 = digitalRead(14);
  byte bt8 = digitalRead(18);

  if (bt1) {
    StartTime = millis();
    StartFlag = 1;
  }

  if (StartFlag == 1) {
    if (millis() - StartTime < 2000) {
      THROTTLE = 30;
      CoDrone.Control();
    }
      else if (millis() - StartTime < 5000) {
      PITCH = 70;
      CoDrone.Control();
    }
      else if (millis() - StartTime < 7000){
        ROLL = 40;
        CoDrone.Control();
      }
    else {
      CoDrone.FlightEvent(Landing);
      StartFlag = 0;
    }
  }
  
  if (bt8) {
     CoDrone.FlightEvent(Stop);
     StartFlag = 0;
}







