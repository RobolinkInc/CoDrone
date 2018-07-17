 /*****************************************************************
  Control - Analog Joystick Control
  This is a basic remote control program for the drone in flight mode
*******************************************************************/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup()
{  
  CoDrone.begin(115200);  // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  //connect
  CoDrone.pair();
  //connect with the nearest drone
  //CoDrone.pair(Nearest);

  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode
}

void loop()
{
  byte bt1 = digitalRead(11);
  byte bt2 = digitalRead(12);
  byte bt3 = digitalRead(13);
  byte bt4 = digitalRead(14);
  byte bt6 = digitalRead(16);
  byte bt7 = digitalRead(17);
  byte bt8 = digitalRead(18);

  // Stop when the left butotn is pressed, but none of the others are
  if (bt1 && !bt4 && !bt8)
  {
//    CoDrone.FlightEvent(Stop);
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (!bt1 && !bt4 && bt8)
  {
     CoDrone.land();
  }
  if (bt2 && bt3 && bt4 && bt6 && bt7)
  {
    CoDrone.calibrate();    
  }
  
  // PAIRING is set to true in AutoConnect if they connect together
  if (PAIRING == true)  // Check to see that we are paired before trying to fly               
  {
    // reads the joystick (analogRead(A#)) and then converts it into a value that the drone can use (AnalogScaleChange())
    // if any of the joysticks move backwards compared to the drone (up is down or left is right), add -1 * at the start
    // it will change the direction the drone moves when you move the joystick
    YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // YAW (turn left or right), port A3, reversed
    THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // THROTTLE (height), port A4, not reversed
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // ROLL (tilt left or right), port A5, reversed
    PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // PITCH (tilt front or back), port A6, not reversed
    
    CoDrone.Control(SEND_INTERVAL); // sends the values to the codrone, it will make sure that SEND_INTERVAL (~50ms) time has passed before it sends again 
  }
}