/*------------------------------------------------------------------
Turn
Objective   : How to Use turnDegree()
syntax      : turnDegree("direction", "angle")
description : TurnDegree is for make drone turn with specific angle
			  Angle can be ANGLE_30, ANGLE_45, ANGLE_60, ANGLE_90,
			  ANGLE_ 120, ANGLE_135, ANGLE_150, ANGLE_180. you actually
			  can use number but have to be int type and bigger than 10.
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                	// Take off and hover for 3 seconds for little auto calibrate
  CoDrone.turnDegree(LEFT, ANGLE_45);   // Turn left 45 degrees
  CoDrone.land();                   	// Landing with sound.
}

void loop() {

}