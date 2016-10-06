# CoDrone
//A collection of CoDrone Libraries and Updates
//Written in Arduino IDE
//For the Robolink community 
//Learn more at www.Robolink.com
//
//
//Collection includes:
//CoDrone.h
//CoDrone.cpp
//EEPROM.h
//EEPROM.cpp
//And various examples for each library


/*******************************************************************/
PATCH NOTES
/*******************************************************************/
1.4.4
---------------------------------------------------------------------
Added Functions:
- There is a known issue where the CoDrone will not run any command for longer 
than ~1 second. This is likely due to an internal timeout function on the CoDrone
firmware. Because of this, using delay() for basic autonomous functions does not work. 
Ideally, the following code would send the CoDrone flying upwards for 5 seconds:
      THROTTLE = 100;
      CoDrone.Control();
      delay(5000);
but the timeout issue would make this CoDrone only fly upwards for ~1 second.
To circumvent this, a new CoDrone.Delay function was created.

 + CoDrone.Delay(int interval);
   - Repeatedly sends the last throttle, roll, pitch and yaw commands once every
   500 milliseconds for the duration of the interval
   - Uses a while loop to imitate delay
   - New class variables Prevroll, Prevyaw, Prevpitch, Prevthrottle
   
 + CoDrone.ControlTime(int interval);    //This function is still in progress, needs to be finished
   - Will act as a combination between CoDrone.Control() and CoDrone.Delay().  
   - The intention is to eliminate the 'middle man' and just have the desired delay
   interval included in the Control() field.
   - Ex. CoDrone.Control(3000) would be equal to CoDrone.Control() + delay(3000);
   

Performance Fixes:
- The disconnect issue is still not completely solved.  I've noticed it happening often with Battle code. 
I believe this is because of the BattleReceive() function that is called every loop.  There is no CheckTime()
included in Receive(), which is called by BattleReceive().  I use TimeCheck3() to limit send intervals to 5ms,
#define RECEIVE_CHECK_TIME
