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

 
unsigned long StartTime;     
int Step;                 // The step you are at in the flight sequence
          

void setup() {  
  CoDrone.begin(115200);  
  CoDrone.pair();

  CoDrone.takeoff();

  Step = 0;  //Initialize step to zero

}

void loop() {  
  byte bt1 = digitalRead(11);  
  byte bt8 = digitalRead(18);  

  // Your killswitch using the far left bottom sensor
  if (bt1) {    
    CoDrone.emergencyStop();
    Step = 0;  
  }  

  // By covering the far right bottom sensor,  you trigger the steps by adding 1 to the 'Step' variable
  if (!bt1 && bt8) {    
    Step = Step + 1;    
        
    if (Step > 3) {          // Set the flightStep back to 0, so you can start over
        Step = 0;    
    }

    CoDrone.Buzz(3000, 7);          // Trigger a buzz sound for activating the next step
    delay(200);  
  }  

  // In the first step, the drone will throttle up at 50% power
  if (Step == 1) {    
    CoDrone.setPitch(0);
    CoDrone.setThrottle(50);           
    CoDrone.move();
  }  

  // In the second step, the drone will pitch forward at 70% power
  else if (Step == 2) {    
    CoDrone.setThrottle(0);
    CoDrone.setPitch(70);
    CoDrone.move();
  }  

  //In the third step, the drone will land and reset to zero
  else if(Step == 3) {
    CoDrone.land();
    Step = 0;
           
  }
  
}