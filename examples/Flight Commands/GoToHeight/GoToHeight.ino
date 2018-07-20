/*------------------------------------------------------------------
Go
Objective   : How to Use goToHeight function
syntax      : goToHeight("height")
description : goToHeight example is using goToHeight() for fly certain height
              You can set 100 ~ 2000 boundary 
              Distance is checked from IR distance sensor next to Optical flow sensor(bottom )
-------------------------------------------------------------------*/

#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.goToHeight(1000);			    // Fly 1000mm away from bottom and hover
  CoDrone.go(FORWARD);           	 	// Go forward at 50% power for 1 second
  CoDrone.goToHeight(400);	     		// Fly 400mm away from bottom and hover
  CoDrone.go(FORWARD);			       	// Go forward at 50% power for 1 second
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}