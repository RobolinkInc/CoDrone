/*------------------------------------------------------------------
ChangeLEDMode
Objective   : How to use ChangeDefaultLEDMode (arm , eye)
syntax      : setArmMode("mode")  * eye mode : LED_NONE, LED_FLICKER, LED_MIX, LED_FLICKERDOUBLE, LED_DIMMING
  			  setEyeMode("mode")  * arm mode: eye mode + LED_FLOW, LED_FLOWREVERSE
  			  setAllMode("mode")
description : ChangeDefaultLEDColor is use three different function
			  to change drone's LED mode. 
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.setArmMode(LED_HOLD) // Change Arm LED mode to hold LED.
  CoDrone.setEyeMode(LED_HOLD) // Change Eye LED mode to hold LED.
  CoDrone.setAllMode(LED_HOLD) // Change Arm and Eye LED mode to hold LED.
  
}

void loop() {

}