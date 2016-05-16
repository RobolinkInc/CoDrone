/*****************************************************************
  PrintDroneAddress()
  This command will print out the address of the last connected drone
  onto the screen. It will appear as:
    0x##, 0x##, 0x##, 0x##, 0x##, 0x##
  Where each # is either 0-9 or A-F

  To see be sure that the drone is still plugged into the computer and
  that the serial monitor (button is the upper right corner) is opened.
  ******************************************************************/

#include <CoDrone.h>  // The codrone library that holds all the background files for this

void setup()
{
  CoDrone.begin(115200);    // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.PrintDroneAddress();  // This prints the drones address onto the screen
}

void loop()
{

}


