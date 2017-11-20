/*****************************************************************
  Turn Over
  This example shows how to do turtle turn 
  (if drone is upsidedown, make right position. Works when drone upsidedown only)
*******************************************************************/
#include <CoDrone.h>   // The codrone library that holds all the background files for this

void setup() 
{ 
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  
  CoDrone.AutoConnect(NearbyDrone); // finds and connects to a drone that is near by
  
  CoDrone.DroneModeChange(Flight); // Changes the drone so that it is now in flght mode
  
  delay(300); // Wait 300milliseconds

  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true) // Check to see that we are paired before trying to fly
  { 
    CoDrone.FlightEvent(TurnOver); //Send turtle turn to drone

    delay(4000);                   // run for 4 seconds (4000 milliseconds)

    CoDrone.FlightEvent(Stop);     //Stop the motor(drone)
  } 
} 

void loop() 
{ 
  
}


