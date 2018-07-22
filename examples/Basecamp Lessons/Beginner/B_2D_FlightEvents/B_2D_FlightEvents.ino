/*------------------------------------------------------------------
This is the sample code for Lesson 2D: Flight Events
https://basecamp.robolink.com/cwists/preview/751x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup() {

  CoDrone.begin(115200);    
  CoDrone.pair();    

  CoDrone.takeoff();                    // Have the drone take off
  delay(2000);                          // Wait (hover) for 2 seconds
  CoDrone.land();                       // Land the drone

}

void loop(){    
  
  // Just assigning variables to 3 of the sensors
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);
  
  // This creates a kill switch when covering button 11
  if(bt1 && !bt4 && !bt8){    
    CoDrone.emergencyStop();
  }

}