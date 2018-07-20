/*------------------------------------------------------------------
Sequence
Objective   : how to use flySequence()
syntax      : flySequence("action")
			  *action can be : ZIGZAG, TRIANGLE, SQUARE, SWAY, SPIRAL, CIRCLE, HOP
description : flySequce function is preset fuction for drone movement.
			  It's not really 
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.flySequence(ZIGZAG);
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}