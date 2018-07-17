/*****************************************************************
  Yaw Up
  This example shows how to control the yaw (rotate left or right)
  Turn right:  1 to 100
  Hold:        0
  Turn left:  -1 to -100
*******************************************************************/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);  // finds and connects to a drone that is near by

  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flght mode
  
  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)  // Check to see that we are paired before trying to fly               
  {
    CoDrone.FlightEvent(TakeOff);        // have the drone take off

    delay(2000);                         // fly for 2 seconds (2000 milliseconds)

    YAW = 100;                           // set YAW to 100 (turn right)
    CoDrone.Control();                   // send the yaw values to the drone

    delay(1000);                         // fly for 1 seconds (1000 milliseconds)

    CoDrone.FlightEvent(Landing);        // land the codrone (when it hits the ground it will stop the motors)
  }
}

void loop()
{
}


