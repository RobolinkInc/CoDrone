/*****************************************************************
  Pitch Up
  This example shows how to control the pitch (tilt to the front or back)
  Tilt forward:   1 to 100
  Level:          0
  Tilt backwards: -1 to -100
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

    PITCH = 100;                         // set PITCH to 100 (tilt forward)
    CoDrone.Control();                   // send the pitch values to the drone

    delay(500);                          // fly for 0.5 seconds (500 milliseconds)

    CoDrone.FlightEvent(Landing);        // land the codrone (when it hits the ground it will stop the motors)
  }
}

void loop()
{

}
