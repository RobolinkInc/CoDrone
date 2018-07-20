/*------------------------------------------------------------------
Hover
Objective	  : How to Use hover()
syntax		  : hover("duration")
description : hover function is simple function make drone hover
			        as same as move("duration",0,0,0,0) it make drone hover
			        for duration time. You can put this function between
			        movement to see movement more clear
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);			// sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);	// Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();				// Take off and hover for 3 seconds for little auto calibrate
  CoDrone.go(FORWARD);				// Go forward at 50% power for 1 second
  CoDrone.hover(3);					// hover for 3 second
  CoDrone.land();					// Landing with sound.
}

void loop() {

}