/*------------------------------------------------------------------
StatusCheck
Objective   : how to program something belong to status
syntax      : is"something"()
			  lowBatteryCheck()
			  *something can be : Flying, ReadytoFly, UpsideDown
description : Status check using if statement to trigger action
			  all function is boolean function so it will return true(not zero)
			  or false(zero)
			  lowbattery is under 40%
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  if (CoDrone.isFlying())		CoDrone.land();
  if (CoDrone.isReadytoFly())	CoDrone.takeoff();
  if (CoDrone.isUpsideDown())	CoDrone.Buzz(550,2);
  if (CoDrone.lowBatteryCheck()) CoDrone.setArmMode(LED_FLICKER);
}

void loop() {

}