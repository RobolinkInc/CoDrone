/*------------------------------------------------------------------
resetDefaultLED
Objective   : How to use resetDefaultLED()
syntax      : resetDefaultLED()	
description : Reset to default LED color
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  // Reset Drone’s LED to holding the Red color 
  CoDrone.resetDefaultLED();
}

void loop() {

}