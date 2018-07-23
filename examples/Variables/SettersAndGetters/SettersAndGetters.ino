/*------------------------------------------------------------------
Setter and Getter
Objective   : how to use getter and setter function
syntax      : get"something"()
              set"something"(number)
              * something can be : Roll, Pitch, Yaw, Throttle
              * number  can be : -100 ~100
description : use setter function to set roll, pitch, yaw, throttle
              instead of putting parameters on move function. use getter
              function to grab current roll pitch yaw throttle
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  int current_roll, current_pitch, current_yaw, current_throllte;

  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate
  CoDrone.setRoll(20);              // set value
  CoDrone.setPitch(0);
  CoDrone.setYaw(20);
  CoDrone.setThrottle(0);
  CoDrone.move(2);
  CoDrone.hover(1);
  current_throllte  = CoDrone.getThrottle(); //get value and save in variables
  current_yaw       = CoDrone.getYaw();
  current_pitch     = CoDrone.getPitch();
  current_roll      = CoDrone.getRoll();
  CoDrone.land();                   // Landing with sound.
}

void loop() {

}