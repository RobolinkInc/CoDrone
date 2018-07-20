/*------------------------------------------------------------------
This is the sample code for Lesson 2I: Buzzers
https://basecamp.robolink.com/cwists/preview/2038x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup(){

  CoDrone.begin(115200);
  CoDrone.pair();

  CoDrone.takeoff();
  delay(2000);
  
  // Buzz, and then throttle upward at 30% power for 1 second
  CoDrone.Buzz(2000, 7);
  CoDrone.setThrottle(30);
  CoDrone.move(1);

  // Buzz, then zero out throttle, and pitch forward at 60% power for 2 seconds
  CoDrone.Buzz(4000, 7);
  CoDrone.setThrottle(0);
  CoDrone.setPitch(60);
  CoDrone.move(2);

  // Buzz, then land
  CoDrone.Buzz(6000,7);
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