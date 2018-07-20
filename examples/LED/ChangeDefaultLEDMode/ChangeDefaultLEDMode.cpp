/*------------------------------------------------------------------
ChangeDefaultLEDMode
Objective   : How to use ChangeDefaultLEDMode (arm , eye)
syntax      : setDefaultArmMode("mode")  * eye mode : LED_NONE, LED_FLICKER, LED_MIX, LED_FLICKERDOUBLE, LED_DIMMING
  			  setDefaultEyeMode("mode")  * arm mode: eye mode + LED_FLOW, LED_FLOWREVERSE
  			  setDefaultAllMode("mode")
description : ChangeDefaultLEDColor is use three different function
			  to change drone's LED mode. and it will remain after you turn on
			  the problem is arduino library can't hold before setting
			  so default for arduino library is LED_HOLD
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.setArmDefaultMode(LED_HOLD) // Change Arm LED mode to hold LED. It will remain after turn off and on again 
  CoDrone.setEyeDefaultMode(LED_HOLD) // Change Eye LED mode to hold LED. It will remain after turn off and on again 
  CoDrone.setAllDefaultMode(LED_HOLD) // Change Arm and Eye LED mode to hold LED. It will remain after turn off and on again 
  
}

void loop() {

}