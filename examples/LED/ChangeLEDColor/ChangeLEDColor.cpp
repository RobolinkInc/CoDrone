/*------------------------------------------------------------------
ChangeLEDColor
Objective   : How to use ChangeLEDColor (arm , eye)
syntax      : setArmLED("red","green","blue")  * using rgb value for choose color
  			  setEyeLED("red","green","blue")  * every rgb value is 0~255
  			  setAllLED("red","green","blue")
description : ChangeDefaultLEDColor is use three different function
			  to change drone's LED. 
			  the problem is arduino library can't hold before setting
			  so default for arduino library is red(255,0,0)
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.setArmLED(0,255,0)  // Change arm LED color to RGB 0, 255, 0(green) 
  CoDrone.setEyeLED(0,255,0)  // Change eye LED color to RGB 0, 255, 0(green) 
  CoDrone.setAllLED(0,255,0)  // Change eye and arm LED color to RGB 0, 255, 0(green) 
}

void loop() {

}