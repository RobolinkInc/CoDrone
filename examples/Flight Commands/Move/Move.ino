/*------------------------------------------------------------------
move
Objective   : How to Use go() in three different ways
syntax      : move() *use saved roll, pitch, yaw, throttle (setter function) and infinity movement
              move("duration")   *use saved roll, pitch, yaw, throttle (setter function)
              move("duration", "roll", "pitch", "yaw", "throttle") 
description : move function is for control the drone with duration, roll
              pitch, yaw, throttle value. It higher version for go() function
              but and can do more like  move forward and left at the sametime
              also duration 0 can be infinity movement
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.move();             		// Move indefinitely based on the current value of flight variables
  CoDrone.move(5);					// Move 5 seconds based on the current value of flight variables            
  CoDrone.move(0, 0, 0, 80, 80);  	// Move up(throttle) and turn left(yaw) indefinitely
  CoDrone.move(5, 0, 0, 80, 80);  	// Move up(throttle) and turn left(yaw) for 5 seconds
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}