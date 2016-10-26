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
-PrintGyro();
  Prints the values of gyroAngle[0], gyroAngle[1]; gyroAngle[2] to the serial monitor
  Uses Send_LinkModeBroadcast to send serial prints between drone communications

-PrintPressure();
  currently non-functional.  No idea why.

-PrintFlow();
  Not working properly.  Returns two values, ImageFlowX and ImageFlowY.  These values only equal -1, 0 and 1. 
  No idea why... 
  Uses 32 bit signed integer.  

**Resolved with Petrone firmware update**  
No changes from follow committed:
<!-- - There is a known issue where the CoDrone will not run any command for longer 
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
   
 + CoDrone.Control(int inverval); 
   - CoDrone.Control() now encompasses a psude 'delay()' function. The new CoDrone.Control(int interval)
    accepts a 'duration' input integer called 'interval'.  The user inputs how long they want their maneuver to
    perform for here instead of using a delay.
      Previous:
      THROTTLE = 60;
      CoDrone.Control();
      delay(3000);
      New:
      THROTTLE = 60;
      CoDrone.Control(3000);
   -The new function resends the previous control command every 500 milliseconds for the duration of the interval.
   -The default empty parameter CoDrone.Control() functions the same way the pervious empty parameter CoDrone.Control() functioned.
    -->

Performance Fixes:
- The disconnect issue is still not completely solved.  I've noticed it happening often with Battle code. 
I believe this is because of the BattleReceive() function that is called every loop.  There is no CheckTime()
included in Receive(), which is called by BattleReceive().  I use TimeCheck3() to limit BattleReceive() intervals to 5ms,
#define RECEIVE_CHECK_TIME
- RECEIVE_CHECK_TIME = 10
