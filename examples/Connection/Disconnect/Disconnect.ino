/*------------------------------------------------------------------
Disconnect
Objective   : How to disconnect with drone and joystick
syntax      : disconnect()
description : This example is showing how to disconnect drone and joystick with coding 
-------------------------------------------------------------------*/

#include<CoDrone.h>     //header
void setup() {
  // Start program and pair with CoDrone.
  CoDrone.begin(115200);
  CoDrone.pair();
  // delay for 2 seconds
  delay(2000);

  //make joystick and CoDrone disconnect
  CoDorne.disconnect();
}
void loop() {

}

