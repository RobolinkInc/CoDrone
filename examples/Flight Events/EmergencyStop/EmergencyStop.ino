/*------------------------------------------------------------------
Calibrate
Objective   : How to use EmergencyStop
syntax      : emergencyStop()
description : emergencyStop is make drone stop everything imediately.
			  if drone got this message drone will stop and fall. 
			  it's little different from landing. landing is make drone
			  Throttle down(go down) and turn off motor when it close to ground
			  but emergencyStop will stop motor right away
			  It's really important for safty feature
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();  
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.emergencyStop();                   // Landing with sound.
}

void loop() {

}