/*****************************************************************
  Control_Custom - 
  This is a controller software that you can completely and 100% customize.
  Use Events["EventName"] to get events;
*******************************************************************/

#include <CoDrone.h> // The codrone library that holds all the background files for this

int Events[] = {"TakeOff" = TakeOff,"Stop" = Stop,"Land" = Landing};

void Refresh() // Setup code; please don't edit.
{
int Joystics[] = {["LX"] = -1 * CoDrone.AnalogScaleChange(analogRead(A3)),["LY"] = CoDrone.AnalogScaleChange(analogRead(A4)),["RX"] = -1 * CoDrone.AnalogScaleChange(analogRead(A5)),["RY"] = CoDrone.AnalogScaleChange(analogRead(A6)),
byte if1 = digitalRead(11); // If sensors are "infrared", on the controller (on the bottom. Look for little lights on 
// the smart inventor board! :)
byte if2 = digitalRead(12);
byte if3 = digitalRead(13);
byte if4 = digitalRead(14);
byte if5 = digitalRead(15);
byte if6 = digitalRead(16);
byte if6 = digitalRead(17);
byte if6 = digitalRead(18);
}

void Control()
{
	Refresh();
	// Your code here! You can use the IF sensors like this:
	// if (if2 && !if3) // (___ = only run code if true, !___ = only run code if false, && = "and", only run code if ___ && ___
	// {[CODE HERE}

	// To change the way the drone is moving:
	// YAW = __ (turn left or right) THROTTLE = __ (height) ROLL = __(tilt left or right) PITCH = __(tilt front or back)
	// To use joysticks, simply do YAW = Joysticks["LX"] (on the left, x-axis.), ROLL = Joysticks["RX"] (right, x-axis), etc.
	// REMEMBER: ADD A ";" AFTER ALL LINES THAT RUN CODE (that means variables (Roll, Pitch, etc), but not logic (if, for, etc)







}
/*****************************************************************
	END EDITING ZONE
	Unless you know what you're doing, it's best to stay above these.
*******************************************************************/
void setup()
{  
  CoDrone.begin(115200);  // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second).

  CoDrone.AutoConnect(NearbyDrone);    // finds and connects to a drone that is nearby.

  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode (Which may or may not be important.)
}

void loop()
{
 if (PAIRING == true)
 {
 Control(); // Runs code
 CoDrone.Control(SEND_INTERVAL); // Sends control (if not already sent; it has a delay anyhow.)


}
