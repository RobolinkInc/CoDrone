/*************************************************************************************
- Rssi Polling
display Rssi signal power(bluetooth connecttion power) on smart board
***************************************************************************************/
#include <CoDrone.h>         // The codrone library that holds all the background files for this             
void setup() 
{ 
  CoDrone.begin(115200);     // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  
  CoDrone.AutoConnect(NearbyDrone);    // finds and connects to a drone that is nearby
  
  CoDrone.DisplayRSSI();            //Show RSSI power using smartboard LEDs
}

void loop()
{
}
