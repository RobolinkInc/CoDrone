/*****************************************************************
  - Throttle Up
  This example shows how to control the throttle
  Rise   :  1 to 100
  Hover  :  0
  Lower  : -1 to -100
  (note your drone might not hover at exactly 0, but it is close)
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
    THROTTLE  = 100;                     // set THROTTLE to 100 (full),
    // note that we do not need to call TakeOff since we are starting the throttle ourselves,
    // but to do so the throttle must be high (recommend 50~75)
    
    CoDrone.Control();                  // send the throttle values to the drone

    delay(2000);                        // fly for 2 seconds (2000 milliseconds)

    CoDrone.FlightEvent(Stop);              // turn off the motors
    //CoDrone.FlightEvent(Landing);         // you could use landing
    //stop might make a damage for drone if drone goes really high
  }
}

void loop()
{
}


