/*------------------------------------------------------------------
This is the sample code for Lesson 2H: Conditionals
https://basecamp.robolink.com/cwists/preview/880x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup(){

  CoDrone.begin(115200);
  CoDrone.pair();

  CoDrone.takeoff();
  delay(2000);

}

void loop(){
  
  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);

  // If pin 18 is covered, pitch forward for 2 seconds
  if(!bt1 && !bt4 && bt8){ 
    CoDrone.setPitch(70);
    CoDrone.move(2);
  }

  else if(bt1 && !bt4 && !bt8){
    CoDrone.emergencyStop();   
  }

}