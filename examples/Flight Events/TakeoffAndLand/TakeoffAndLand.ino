/*------------------------------------------------------------------
Calibrate
Objective   : How to use takeoff() and land()
syntax      : takeoff()
			  land()
description : takeoff() is almost same as previous method flightEvent(TakeOff) 
			  but will do 3 second hover for micro trim(make it stable) 
			  land() is doing samething as flightEvent(Landing)
			  we add sound easy to notice landing
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}