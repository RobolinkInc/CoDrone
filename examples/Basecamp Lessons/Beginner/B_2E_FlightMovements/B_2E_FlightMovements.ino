/*------------------------------------------------------------------
This is the sample code for Lesson 2E: Flight Movements
https://basecamp.robolink.com/cwists/preview/772x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup() {
  CoDrone.begin(115200);
  CoDrone.pair();

  CoDrone.takeoff();                          //Take off for 2 seconds
  delay(2000);

  CoDrone.setThrottle(50);                    // Throttle up for 2 seconds
  CoDrone.move(2);

  CoDrone.setThrottle(0);                     // Set throttle back to 0, pitch and roll for 2 seconds
  CoDrone.setPitch(30);                       
  CoDrone.setRoll(-30);
  CoDrone.move(2);

  CoDrone.land();                             // Land

}

void loop () {
  
  // Just assigning variables to 3 of the sensors
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);
  
  // This creates a kill switch when covering button 11
  if(bt1 && !bt4 && !bt8){
    CoDrone.FlightEvent(Stop);
  }

}