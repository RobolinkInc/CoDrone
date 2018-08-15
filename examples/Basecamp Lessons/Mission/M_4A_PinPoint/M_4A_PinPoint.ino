/*------------------------------------------------------------------
This is the sample code for Lesson 4A: Pin Point
https://basecamp.robolink.com/cwists/preview/2224x

In this mission, the flight sequence relies on a timer rather than using the usual
flight command functions.  This is just a template for you to start from, the actual
mission and route is up to you.  From 0 - 1.5 seconds, it throttles, from 1.5 - 3.5
seconds it pitches, and from 3.5 - 5.5 seconds, it rolls.  You'll notice the 
movements are much more rounded and fluid, because there are no delays between the 
3 time segments.
------------------------------------------------------------------*/

#include <CoDrone.h>

int FLAG;                           // The flag used for determining if you've activated the sequence
unsigned long StartTime;            // Your start time used for checking how much time has passed

void setup() {
  
  CoDrone.begin(115200);
  CoDrone.pair(Nearest);

  CoDrone.takeoff();
  delay(2000);

}

void loop() {

  byte bt1 = digitalRead(11);
  byte bt8 = digitalRead(18);
  
  if (bt1) {
    CoDrone.emergencyStop();
    FLAG = 0; 
  } 

  // When you cover the far right button, buzz, wait 0.2 seconds, then start the timer which will run the sequence
  if (!bt1 && bt8) {    
    FLAG = 1;
    CoDrone.Buzz(5000, 7);
    delay(200);
    StartTime = millis();
  }  

  if ( FLAG == 1) {
    
    // In the first 1.5 seconds, throttle up at 30% power
    if (millis() - StartTime < 1500) {
      CoDrone.setRoll(0);
      CoDrone.setPitch(0);
      CoDrone.setThrottle(30);
      
      CoDrone.move();
    }
    
    // From 1.5 seconds to 3.5 seconds, pitch forward at 40% power
    else if (millis() - StartTime < 3500) {
      CoDrone.setRoll(0);
      CoDrone.setPitch(40);
      CoDrone.setThrottle(0);

      CoDrone.move();
    }

    // From 3.5 seconds to 5.5 seconds roll right at 50% power
    else if (millis() - StartTime < 5500) {
      CoDrone.setRoll(50);
      CoDrone.setPitch(0);
      CoDrone.setThrottle(0);
      
      CoDrone.move(); 
    }

    // At the end of it all, land
    else if (FLAG == 1){ 
      CoDrone.land();
      FLAG = 0;

    }

  }

}