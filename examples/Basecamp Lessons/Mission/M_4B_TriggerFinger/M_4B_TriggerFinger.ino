/*------------------------------------------------------------------
This is the sample code for Lesson 4B: Trigger Finger
https://basecamp.robolink.com/cwists/preview/2221x

In this mission, you are given the ability to trigger the next step of
the flight sequence at will.  The right sensor will trigger the next
step of the flight sequence.  The code provided at step 1 will make the
drone throttle up, then at step 2 will make the drone pitch forward.
In the last step, the drone will land.  You can add as many steps
and maneuvers as you'd like.
------------------------------------------------------------------*/

#include <CoDrone.h>

int flightStep;                      // The step you are at in the flight sequence
boolean initTakeoff = true;          // A boolean you set to determine whether you just took off

void setup() {  
  CoDrone.begin(115200);  
  CoDrone.AutoConnect(NearbyDrone);

  CoDrone.takeoff();

}

void loop() {  
  byte bt1 = digitalRead(11);  
  byte bt8 = digitalRead(18);  

  // Your killswitch using the far left bottom sensor
  if (bt1) {    
    CoDrone.emergencyStop();
    flightStep = 0;  
  }  

  // By covering the far right bottom sensor,  you trigger the steps by adding 1 to the 'Step' variable
  if (!bt1 && bt8) {    
    flightStep = flightStep + 1;    
        
    if (flightStep >= 3) {          // Set the flightStep back to 0, so you can start over
      flightStep = 0;    
    }

    CoDrone.Buzz(3000, 7);          // Trigger a buzz sound for activating the next step
    delay(200);  
  }  

  // In the first step, the drone will throttle up at 50% power
  if ( flightStep == 1) {    
    CoDrone.setPitch(0);
    CoDrone.setThrottle(50);

    // Now that you've taken off, so anything outside of your steps will be caught by
    // the last else-if statement to have the drone land
    initTakeoff = false;            
    
    CoDrone.move();
  }  

  // In the second step, the drone will pitch forward at 70% power
  else if (flightStep == 2) {    
    CoDrone.setThrottle(0);
    CoDrone.setPitch(70);
    
    // This one isn't actually necessary, but it's just setting the initTakeoff to false
    // for consistency's sake.
    initTakeoff = false;
    
    CoDrone.move();
  }  

  // In the last step, the drone will land if it wasn't your initial take off. If you just
  // had a simple "else" without the last else-if check on "initTakeoff," the drone would 
  // immediatley land after having just taken off!
  else if(initTakeoff == false) {
    CoDrone.land();

    // Set this back to true, so that the next time you take off, it will hover until your first step
    initTakeoff = true;             
  }
  
}