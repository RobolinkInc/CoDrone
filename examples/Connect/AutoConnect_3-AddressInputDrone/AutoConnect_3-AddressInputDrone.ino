/*****************************************************************
  - Connect 3
  AutoConnect(droneAddress)
  This connects to a drone using the address given to it. Each drone
  has it own set of 12 numbers and letters that identify it.
*******************************************************************/

#include <CoDrone.h>  // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200);  // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  // CoDrone.PrintDroneAddress();        //  This command can be uncommented and used to see the address of the drone

  byte droneAddress[6] = {0xEC, 0x3F, 0xE5, 0xC2, 0xB5, 0xD0};  // This is the address of the drone, found by the above line of code
  CoDrone.AutoConnect(AddressInputDrone, droneAddress);         // This connects to the drone at that address only
  
  CoDrone.DroneModeChange(Flight); // Changes the drone so that it is now in flight mode
  
  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)  // Check to see that we are paired before trying to fly
  {
    CoDrone.FlightEvent(TakeOff);         // tells the drone to take off
    delay(2000);                          // waits 2 seconds (2000 milliseconds)
    CoDrone.FlightEvent(Landing);         // tells the drone to land and turns off the motors
  }
}

void loop()
{
}


