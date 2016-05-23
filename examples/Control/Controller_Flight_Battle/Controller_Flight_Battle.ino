/*********************************************************************************
  Control - Analog Joystick Control - Drone Battle
  This code will allow you to play a laser tag type battle with multiple CoDrones
   - Touching the bottom left IR sensor will send the Stop command to the drone
   - Touching the bottom right IR sensor will send the Shoot command. 
   - Each player should choose a team. The default is TEAM_BLUE.  Teammates cannot shoot eachother
   - You can select FREE_PLAY for a free-for-all 

   - You start with 8 hit points. Being shot reduces your hit points by one
   - You lose the game when you have 0 hit points left
   - Your current hit points are shown on the Smart Inventor board LEDs
   - The controller will emit a loss tone when you are out of hit points
***********************************************************************************/

#include <CoDrone.h> // Include the CoDrone library

void setup()
{
  CoDrone.begin(115200);                // Start the serial connection at (115200bps)
  CoDrone.AutoConnect(NearbyDrone);    // Connect to a nearby drone
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
    CoDrone.FlightEvent(Stop);     // Stop the drone
  }

  if (!bt1 && !bt4 && bt8)           // If the right IR sensor is triggered
  {
    CoDrone.BattleShooting();        // Shoot
    CoDrone.ButtonPreesHoldWait(18); // Delay.  So you can't just keep shooting.
  }

  if (!bt1 && bt4 && !bt8)            // If the center IR sensor is triggered
  {
    CoDrone.FlightEvent(Landing);    // Perform the landing manuever
  }

  YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // Set the A3 analog pin to control the Yaw
  THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // Set the A4 analog pin to control the Throttle
  ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // Set the A5 analog pin to control the Roll
  PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // Set the A6 analog pin to control the Pitch
  CoDrone.Control(SEND_INTERVAL);                         // Send the new flight commands at the SEND_INTERVAL (50ms)

}
