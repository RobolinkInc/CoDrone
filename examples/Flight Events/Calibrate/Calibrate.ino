/*------------------------------------------------------------------
Calibrate
Objective   : How to Calibrate programatically
syntax      : calibrate()
description : calibrate is doing samething as a reset the drone.
			  We just got update since version 50.18 to active this function
			  you can send this function and make gradual sound and try
			  to calibrate and land. it will show yellow light blinking
			  after finish calibrate if you wanna do again pull out the 
			  battery and put it back
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();  
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.calibrate();				// after green arm LED blink do some calibartion
}

void loop() {

}