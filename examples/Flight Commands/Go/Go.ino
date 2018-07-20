/*------------------------------------------------------------------
Go
Objective	  : How to Use go() in three different ways
syntax		  : go("Direction") 	* duration 1 sec, 50% of power.
			        go("direction", "duration") *50% of power.
			        go("direction", "duration", "power")
description : Go function is for control the drone with more nature language.
			        as you can see in example you and put "Direction" like FORWARD,
			        BACKWARD, LEFT, RIGHT, UP, DOwN. second parameter is duration.
			        If you put positive number for set duration. 
			        last parameter is power you can set 0~100
-------------------------------------------------------------------*/

#include <CoDrone.h>

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module Baudrate at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.go(FORWARD);       	 	// Go forward at 50% power for 1 second
  CoDrone.go(UP, 5);          		// Go up for 5 seconds at 50% power
  CoDrone.go(BACKWARD, 3, 70) 		// Go backwards for 3 seconds at 70% power
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}