/*------------------------------------------------------------------
Battle Controller
Objective   : How to code Controller program with shooting IR shooting play
syntax      : 
description : This extend version flight Controller example.
              if you select team you can see with arm and eye LED color changed
              add Battle shooting key is includeed programatically
-------------------------------------------------------------------*/
#include <CoDrone.h> // Include the CoDrone library

void setup()
{
  CoDrone.begin(115200);                // Start the serial connection at (115200bps)
  
  //connect with drone controller have adress
  CoDrone.pair();
  //connect with the nearest
  CoDrone.DroneModeChange(Flight);      // Make sure the drone is in Flight mode
  CoDrone.BattleBegin(TEAM_BLUE);       //  Choose a team : TEAM_RED  / TEAM_BLUE /  TEAM_GREEN  / TEAM_YELLOW / FREE_PLAY
}

void loop()
{
  CoDrone.BattleReceive();        // Receive the IR signals from the other drone

  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   Initialize the bottom left IR sensor
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   Initialize the bottom center IR sensor
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   Initialize the bottom right IR sensor

  if (bt1 && !bt4 && !bt8)          // If the left IR sensor is triggered
  {
    CoDrone.emergencyStop();
  }

  if (!bt1 && !bt4 && bt8)           // If the right IR sensor is triggered
  {
    CoDrone.BattleShooting();        // Shoot.  There is a 300 millisecond inteveral between shots.
  }

  if (!bt1 && bt4 && !bt8)            // If the center IR sensor is triggered
  {
    CoDrone.land();
  }

  YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // Set the A3 analog pin to control the Yaw
  THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // Set the A4 analog pin to control the Throttle
  ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // Set the A5 analog pin to control the Roll
  PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // Set the A6 analog pin to control the Pitch
  CoDrone.move(ROLL,PITCH,YAW,THROTTLE);                  // Send the new flight commands

}