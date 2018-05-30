// This is the sample code for Lesson 3A:Flags
// https://basecamp.robolink.com/cwists/preview/767x

#include<CoDrone.h>

int move_flag;
int fly_flag;
int counter;

void setup() {
 CoDrone.begin(115200);
 CoDrone.AutoConnect(NearbyDrone);
 CoDrone.DroneModeChange(Flight); 
 
 move_flag = 0;
 fly_flag = 0;
 counter = 0;
}

void loop() {
 byte bt1 = digitalRead(11);
 
 if( bt1 ){
   CoDrone.FlightEvent(Stop);
 }

 delay(1000);
 counter++;

 
 if(counter == 3){
   fly_flag = 1;
 }

 if(counter == 6){
   move_flag = 1;
 }

 if(counter == 9){
   PITCH = 0;
   CoDrone.Control();
   delay(500);
   CoDrone.FlightEvent(Landing);
 }

 
 if(fly_flag == 1){
   CoDrone.FlightEvent(TakeOff);
   fly_flag = 0;
   CoDrone.Buzz(400,4);
 }

 if(move_flag == 1){
   PITCH = 50;
   CoDrone.Control();
   CoDrone.Buzz(400,4);
 }
 
}







