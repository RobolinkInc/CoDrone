/*------------------------------------------------------------------
ChangeDefaultLEDColor
Objective   : How to use ChangeDefaultLEDColor (arm , eye)
syntax      : setDefaultArmLED("red","green","blue")  * using rgb value for choose color
  			  setDefaultEyeLED("red","green","blue")  * every rgb value is 0~255
  			  setDefaultAllLED("red","green","blue")
description : ChangeDefaultLEDColor is use three different function
			  to change drone's LED. and it will remain after you turn on
			  the problem is arduino library can't hold before setting
			  so default for arduino library is red(255,0,0)
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.setDefaultArmLED(0,255,0)  // Change arm Default LED color to RGB 0, 255, 0(green) 
  CoDrone.setDefaultEyeLED(0,255,0)  // Change eye Default LED color to RGB 0, 255, 0(green) 
  CoDrone.setDefaultAllLED(0,255,0)  // Change eye Default and arm LED color to RGB 0, 255, 0(green) 
}

void loop() {

}