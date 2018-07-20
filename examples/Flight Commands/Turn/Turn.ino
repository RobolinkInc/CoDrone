/*------------------------------------------------------------------
Turn
Objective   : How to Use turn() in three different ways
syntax      : turn("direction") *duration will be 1 sec and power will be 50%
              turn("direction", "duration")   *power will be 50%
              turn("direction", "duration", "power")
description : turn function is for make drone turn(yawing) using natural language
			  Using duration fur turning time and use power for turn speed
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.turn(LEFT);             	// Turn left for 1 second at 50 power
  CoDrone.turn(LEFT, 0);          	// Turn left at 50 power indefinitely
  CoDrone.turn(RIGHT, 5, 100);     	// Turn right for 5 seconds at 100 power
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}