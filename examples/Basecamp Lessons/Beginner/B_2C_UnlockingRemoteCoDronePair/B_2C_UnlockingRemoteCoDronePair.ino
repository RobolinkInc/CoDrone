/*------------------------------------------------------------------
This is the sample code for Lesson 2C: Unlocking a Remote-CoDrone Pair
https://basecamp.robolink.com/cwists/preview/3435x
------------------------------------------------------------------*/

#include <CoDrone.h>

void setup(){

  CoDrone.begin(115200);
  // Pair with the first drone found. Use this (instead of pair() ) if you are trying to pair 
  // with a different drone than your usual drone.
  CoDrone.pair(Nearest);
  
}

void loop(){

}