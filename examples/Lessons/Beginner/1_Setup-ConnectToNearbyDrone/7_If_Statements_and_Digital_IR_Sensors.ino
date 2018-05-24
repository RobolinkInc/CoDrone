// This is the sample code for Lesson 2G: If Statements and Digital IR Sensors
// https://basecamp.robolink.com/cwists/preview/872x

#include <CoDrone.h>

void setup(){
   CoDrone.begin(115200);
   CoDrone.AutoConnect(NearbyDrone);
}

void loop(){ 
    byte bt8 = digitalRead(18);
    byte bt4 = digitalRead(14);
    byte bt1 = digitalRead(11);

    if(bt8){
     CoDrone.FlightEvent(TakeOff);
     delay(2000);
     CoDrone.FlightEvent(Landing);
   }

   else if(bt1 && bt4 && !bt8){
      CoDrone.FlightEvent(Stop);
    }
}







