/*****************************************************************
  Control - Analog Joystick Control
  This program is for the drone when it is in the drive mode (wheels)
  It is a basic remote control program it uses one joystick
*******************************************************************/
#include <CoDrone.h>  // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200);   // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);      // finds and connects to a drone that is near by

  CoDrone.DroneModeChange(Drive);        // Changes the drone so that it is now in drive (wheels) mode
}

void loop()
{
  byte bt1 = digitalRead(11);       // reads the far left button and saves it to btn1
  byte bt4 = digitalRead(14);       // reads the middle button and saves it to btn4
  byte bt8 = digitalRead(18);       // reads the far right button and saves it to btn8

  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)  // Check to see that we are paired before trying to fly               
  {
    // reads the joystick (analogRead(A#)) and then converts it into a value that the drone can use (AnalogScaleChange())
    THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // Throttle (forward/reverse) is on port A4
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // Roll (turning) is on port A5 and needs to be reversed (-1 *)
    
    CoDrone.Control(SEND_INTERVAL);   // sends the values to the codrone, it will make sure that SEND_INTERVAL (~50ms) time has passed before it sends again 
  }
}


