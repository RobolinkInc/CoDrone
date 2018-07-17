/*****************************************************************
  - Connect 2
  AutoConnect(ConnectedDrone)
  This shows how to connect to a previously connected drone.
  It will connect only to the last drone it was connected to.
*******************************************************************/

#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200);                // starts the communication with the drone through the bluetooth module at 115200bps

  CoDrone.AutoConnect(ConnectedDrone);  // Connects to the last drone that it connected to
  
  CoDrone.DroneModeChange(Flight);       // Changes the drone so that it is now in flight mode
  
  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)                    // Check to see that we are paired before trying to fly
  {
    CoDrone.FlightEvent(TakeOff);        // tells the drone to take off
    delay(2000);                         // waits 2 seconds (2000 milliseconds)
    CoDrone.FlightEvent(Landing);        // tells the drone to land and turns off the motors
  }
}

void loop()
{
  
}
