/*****************************************************************
  Trim - Trim all
- Use this code to trim your drone if you already know what trim values you want to use

Set the integer values for roll, pitch, yaw, and throttle (-100 to 100) and they will
be added as trim to your drone when it connects.

*******************************************************************/
#include <CoDrone.h> // Include the CoDrone Library

// range -100 ~ 100

int roll      = 0;
int pitch     = 0;
int yaw       = 0;
int throttle  = 0;
int wheel     = 0;     //use drive mode 

void setup()
{  
  CoDrone.begin(115200);                // Begin serial connections to the bluetooth board (115200bps)
  CoDrone.AutoConnect(NearbyDrone);     // Connect to a nearby CoDrone
 
  CoDrone.Set_TrimAll(roll, pitch, yaw, throttle, wheel);  
  
}

void loop()
{
  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   Initialize the bottom left IR sensor
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   Initialize the bottom center IR sensor
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   Initialize the bottom right IR sensor

  if (bt1 && !bt4 && !bt8)           // If the left IR sensor is triggered
  {
    CoDrone.FlightEvent(Stop);      // Stop the drone
    // CoDrone.FlightEvent(Landing); // Or land the drone.
  }

  if (PAIRING == true)                // If the drone is properly paired
  {
    YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // Set the A3 analog pin to control the Yaw
    THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // Set the A4 analog pin to control the Throttle
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // Set the A5 analog pin to control the Roll
    PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // Set the A6 analog pin to control the Pitch
    CoDrone.Control(SEND_INTERVAL);                         // Send the new flight commands at the SEND_INTERVAL (50ms)
  }
}


