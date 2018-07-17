/*****************************************************************
  - Connect 1
  AutoConnect(NeardbyDrone)
  This will automatically find and connect to a drone that is near by
*******************************************************************/

#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200);  // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);    // finds and connects to a drone that is nearby
  
  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode
  
  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)  // Check to see that we are paired before trying to fly
  {
    CoDrone.FlightEvent(TakeOff);        // tells the drone to take off
    delay(2000);                         // waits 2 seconds (2000 milliseconds)
    CoDrone.FlightEvent(Landing);        // tells the drone to land and turns off the motors
  }
}

void loop()
{

}


