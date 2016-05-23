/*****************************************************************
  Trim - Analog Joystick Control + Trim
  This code will help you trim your CoDrone.

- Touching the left sensor will send the Stop command to your drone
- Covering the right sensor will engage Trim Mode
- Covering both the left and right sensors will reset all trim values to 0

How to Trim:
When NOT covering the bottom right IR sensor, the controller will act as a normal controller. 
Use this mode to test your CoDrone and see what trim is necassary.  If your drone tends to lean
foward (pitch foward) then you will want to add a negative pitch trim.  If your drone tends to 
lean diagonally to the back right (pitch backward and roll right) then you will want to add
a positive pitch trim and a negative roll trim. 

To add trim, simply cover the right IR sensor with your finger and push the joystick fully in the direction
of the desired trim.  Trim adds incrementally, so if you want to add a lot of trim just tap the joystick 
multiple times.  To properly add trim, you must be covering the right IR sensor while you move the joystick 
in the trim direction.  The trim values will stay saved in the drone even after you turn off the controller 
and change programs.

Example:
My drone does not fly straight up.  When I throttle upwards, it always tilts to the front and right. 
--Solution--
Upload this trim code.  Cover the right IR sensor with your finger and (while the sensor is still covered)
tap the pitch joystick (right joystick by default) all the way to the bottom three times.  Uncover the right
IR sensor.  Test the drone again by sending throttle (left joystick by default).  The drone now only tilts
to the right (the negative pitch trim solved the forward tilt).  Cover the right IR sensor with your finger
and tap the roll joystick (right joystick by default) all the way to the left three times.  Uncover the right
IR sensor.  Test the drone again.  The drone now flies straight up and hovers without tilt.  Turn the drone
and controller off, and upload any program you want. The trim will stay. 


*******************************************************************/

int PreviousEventPitch = 0;
int PreviousEventThrottle = 0;
int PreviousEventYaw = 0;
int PreviousEventRoll = 0;

#include <CoDrone.h> // Include the CoDrone library 

void setup()
{
  CoDrone.begin(115200);                // Begin the serial connection with the CoDrone Bluetooth module(115200bps)
  CoDrone.AutoConnect(NearbyDrone);     // Connect to a nearby drone
  CoDrone.DroneModeChange(Flight);      // Ensure that the drone is in flight mode
}

void loop()
{
  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   Initialize the bottom left IR sensor
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   Initialize the bottom center IR sensor
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   Initialize the bottom right IR sensor

  if (PAIRING == true)              // If the drone has succesfully paired
  {

    //**************************** STOP ******************************//
    //■ □ □ □ □ □ □ □
    if (bt1 && !bt4 && !bt8)           // If the left sensor is triggered
    {
      CoDrone.FlightEvent(Stop);      // Stop the CoDrone
      // CoDrone.FlightEvent(Landing);      // Land the CoDrone.  You can choose either of these.
    }

    //*************************** Trim Reset ***********************//
    //■ □ □ □ □ □ □ ■
    else if (bt1 && !bt4 && bt8)  // If both the left and right sensors are triggered
    {
      CoDrone.Set_TrimReset();    // Reset all trim values
      CoDrone.LED_Blink(25, 5);
      CoDrone.LED_Connect();
      CoDrone.ButtonPreesHoldWait(11, 18); //button up
    }

    //*************************** Control & Trim  ***********************//
    //□ □ □ □ □ □ □ ■
    if (!bt8)  //If the right sensor is NOT triggered
    {
      digitalWrite(16, HIGH);
      YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // Set the A3 analog pin to control the Yaw
      THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // Set the A4 analog pin to control the Throttle
      ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // Set the A5 analog pin to control the Roll
      PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // Set the A6 analog pin to control the Pitch
      CoDrone.Control(SEND_INTERVAL);                         // Send the new flight commands at the SEND_INTERVAL (50ms)

    }


    else if (bt8)  // If the right sensor IS triggered
    {
      digitalWrite(16, LOW);
      YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));  //Set the joysticks to control the trim instead of the drone itself
      THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));
      ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));
      PITCH = CoDrone.AnalogScaleChange(analogRead(A6));

      if (PITCH == 0) PreviousEventPitch = 0;                //Instead of using 'SEND_INTERVAL', we use the input values as triggers
      if (THROTTLE == 0) PreviousEventThrottle = 0;
      if (ROLL == 0) PreviousEventRoll = 0;
      if (YAW == 0) PreviousEventYaw = 0;

      if ((PITCH != 0) || (THROTTLE != 0) || (ROLL != 0) || (YAW != 0))
      {
        CoDrone.LED_Blink(20, 3);
        CoDrone.LED_Connect();
        digitalWrite(16, LOW);
      }

      if (PITCH > 90)                                    //If the Pitch value is high
      {
        if (PreviousEventPitch != PitchIncrease)         //Increase the Pitch Trim
        {
          PreviousEventPitch = PitchIncrease;
          CoDrone.Set_Trim(PitchIncrease);
        }
      }

      else if (PITCH < -90)                             //else if the Pitch value is low
      {
        if (PreviousEventPitch != PitchDecrease)        //Decrease the Pitch Trim
        {
          PreviousEventPitch = PitchDecrease;
          CoDrone.Set_Trim(PitchDecrease);
        }
      }

      else if (THROTTLE > 90)                           //And so on and so forth
      {
        if (PreviousEventThrottle != trim_ThrottleIncrease)
        {
          PreviousEventThrottle = trim_ThrottleIncrease;
          CoDrone.Set_Trim(trim_ThrottleIncrease);
        }
      }

      else if (THROTTLE < -90)
      {
        if (PreviousEventThrottle != trim_ThrottleDecrease)
        {
          PreviousEventThrottle = trim_ThrottleDecrease;
          CoDrone.Set_Trim(trim_ThrottleDecrease);
        }
      }

      else if (ROLL > 90)
      {
        if (PreviousEventRoll != RollDecrease)
        {
          PreviousEventRoll = RollDecrease;
          CoDrone.Set_Trim(RollDecrease);
        }
      }
      else if (ROLL < -90)
      {
        if (PreviousEventRoll != RollIncrease)
        {
          PreviousEventRoll = RollIncrease;
          CoDrone.Set_Trim(RollIncrease);
        }
      }

      else if (YAW > 90)
      {
        if (PreviousEventYaw != YawIncrease)
        {
          PreviousEventYaw = YawIncrease;
          CoDrone.Set_Trim(YawIncrease);
        }
      }
      else if (YAW < -90)                     //Cats are cooler than dogs
      {
        if (PreviousEventYaw != YawDecrease)
        {
          PreviousEventYaw = YawDecrease;
          CoDrone.Set_Trim(YawDecrease);
        }
      }
    }
  }
}





