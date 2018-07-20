/*------------------------------------------------------------------
// This is the sample code for Lesson 3A:Flags
// https://basecamp.robolink.com/cwists/preview/767x
-------------------------------------------------------------------*/

#include<CoDrone.h>

int move_flag;
int fly_flag;
int counter;

void setup() {
  CoDrone.begin(115200);
  CoDrone.pair();     // Pair with one the last drone this remote paired with
  CoDrone.DroneModeChange(Flight); 
 
  move_flag = 0;
  fly_flag = 0;
  counter = 0;
}

void loop() {
  byte bt1 = digitalRead(11);
 
  if( bt1 ){
    CoDrone.emergencyStop();  //kill switch
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
    CoDrone.setPitch(0);
    CoDrone.Control();
    delay(500);
    CoDrone.land();
  }
  
  //flag trigger takeoff the drone
  if(fly_flag == 1){     
    CoDrone.takeoff();
    fly_flag = 0;
  }

  // move trigger make drone go forward
  if(move_flag == 1){
    CoDrone.setPitch(50);
    CoDrone.move();
    CoDrone.Buzz(400,4);
  }
 
}