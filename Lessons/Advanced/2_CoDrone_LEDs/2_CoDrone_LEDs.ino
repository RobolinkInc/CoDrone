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

  if (bt1) {
    StartFlag = 1;
    StartTime = millis();
  }

  if (StartFlag == 1) {
    if (millis() - StartTime < 1000) { 
      CoDrone.LedColor(EyeDimming, White, 20);
      THROTTLE = 50;
      CoDrone.Control();
    }
    else if ( millis() - StartTime < 3000) {
      CoDrone.LedColor(EyeDimming, Blue, 20);
      PITCH = 50; 
      CoDrone.Control(); 
    }
    else { 
      CoDrone.FlightEvent(Landing);
      StartFlag = 0; 
    }
  }
}







