/*------------------------------------------------------------------
This is the sample code for Lesson 2B: Communication and Pairing
https://basecamp.robolink.com/cwists/preview/720x
------------------------------------------------------------------*/

#include <CoDrone.h> 

void setup() 
{ 

  CoDrone.begin(115200);
  CoDrone.pair();				// Pair with one the last drone this remote paired with

} 

void loop() { 
  
}