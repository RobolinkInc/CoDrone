/*------------------------------------------------------------------
This is the sample code for Lesson 2G: If Statements and Digital IR Sensors
https://basecamp.robolink.com/cwists/preview/872x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup(){
  
  CoDrone.begin(115200);
  CoDrone.pair();

}

void loop(){

  byte bt8 = digitalRead(18);
  byte bt4 = digitalRead(14);
  byte bt1 = digitalRead(11);

  // Once pin 18 is triggered, do a takeoff, wait 2 seconds, then land
  if(bt8){
    CoDrone.takeoff();
    delay(2000);
    CoDrone.land();
  }

  else if(bt1 && !bt4 && !bt8){
    CoDrone.FlightEvent(Stop);
  }

}