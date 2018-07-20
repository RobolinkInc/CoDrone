/*------------------------------------------------------------------
This is the sample code for Lesson 2F: Variables
https://basecamp.robolink.com/cwists/preview/865x
------------------------------------------------------------------*/

#include <CoDrone.h>

// Set a bunch of variables to different integer values
int A = 1000;
int B = 500;
int C = 5;
int D = 2;
int E = 1;
int F = 3;

void setup(){  
  CoDrone.begin(115200);  
  CoDrone.pair();  
  CoDrone.takeoff();
  delay(A);                                   // Wait 1 second

  CoDrone.setThrottle((C * C) * F);           // 5 * 5 * 3  = 75 power
  CoDrone.move(E);                            // move at 75% power throttle for 1 seconds

  CoDrone.setThrottle(0);                     // Zero out the throttle
  CoDrone.move();

  CoDrone.land();

}

void loop(){
  
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);

  if(bt1 && !bt4 && !bt8){
    CoDrone.emergencyStop();
  }

}