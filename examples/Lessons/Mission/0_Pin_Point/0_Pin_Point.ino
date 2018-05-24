// This is the sample code for Lesson 4A: Pin Point
// https://basecamp.robolink.com/cwists/preview/2224x

#include <CoDrone.h>

int FLAG;
unsigned long StartTime;

void setup() {
  CoDrone.begin(115200);
  CoDrone.AutoConnect(NearbyDrone);

  CoDrone.FlightEvent(TakeOff);
  delay(2000);
}

void loop() {
  byte bt1 = digitalRead(11);
  byte bt8 = digitalRead(18);
  if (bt1) {
    CoDrone.FlightEvent(Stop);
    FLAG = 0; 
 } 
 if (!bt1 && bt8) 
{    FLAG = 1;
    CoDrone.Buzz(5000, 7);
    delay(200);
    StartTime = millis();
  }  
if ( FLAG == 1) {
    if (millis() - StartTime < 1500) {
      THROTTLE = 70; 
     CoDrone.Control();
    }
    else if (millis() - StartTime < 3500) {
      PITCH = 40;
      CoDrone.Control();
    }
    else if (millis() - StartTime < 5500) {
      ROLL = 50;
      CoDrone.Control(); 
   }    else { 
     CoDrone.FlightEvent(Landing);
    }
  }
}
