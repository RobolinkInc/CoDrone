/*****************************************************************
  - LowBatteryCheck
  LowBatteryCheck(level)
  Cecks the battery level of the drone
  * battery percentage: 0 to 100
  * if battery is less than level it will give a warning
*******************************************************************/

#include <CoDrone.h> // The codrone library that holds all the background files for this

byte level = 50;    // the minimum batery level

void setup()
{
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);     // finds and connects to a drone that is nearby
  
  CoDrone.LowBatteryCheck(level);       // check if the battery is below level (50) percent
 
}

void loop()
{
}
