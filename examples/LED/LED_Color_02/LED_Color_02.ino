/*****************************************************************
  - LED Color 2
  Second way to control the leds
  LedColor(Mode,R,G,B,Time);
    Mode: which leds to control and what pattern (select from first list below)
    R: Red value of the color (0-255, with 0 being no red, and 255 being a lot of red)
    G: Green value of the color (0-255, with 0 being no green, and 255 being a lot of green)
    B: Blue value of the color (0-255, with 0 being no blue, and 255 being a lot of blue)
    Time: length of time of each pattern (0-255)
*******************************************************************/
#include <CoDrone.h> // The codrone library that holds all the background files for this

byte modeTime = 7;                        // How long each pattern takes
int delayTime = 1000;                     // How long to wait before changing light patterns

void setup()
{
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);     // finds and connects to a drone that is nearby
}

void loop()
{
  CoDrone.LedColor(ArmDimming, 255, 0, 0, modeTime);   // Set the arm leds to red, dimming every modeTime
  delay(delayTime);                                    // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, 0, 255, 0, modeTime);   // Set the arm leds to green, dimming every modeTime
  delay(delayTime);                                    // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, 0, 0, 255, modeTime);   // Set the arm leds to blue, dimming every modeTime
  delay(delayTime);                                    // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, 180, 200, 0, modeTime); // Set the arm leds to purple, dimming every modeTime
  delay(delayTime);                                    // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, 0, 0, 0, modeTime);     // turn off the leds
  delay(delayTime);                                    // wait delayTime (1000)
}

/*********************************************************************************************************************************
    led mode options
  EyeNone           ///< Eyes, to blank
  EyeHold,          ///< Eyes, constant color
  EyeMix,           ///< Eyes, mixed pattern/color
  EyeFlicker,       ///< Eyes, Flickering
  EyeFlickerDouble, ///< Eyes, Flickering twice
  EyeDimming,       ///< Eyes, dimming

  ArmNone            ///< Arms, to blank
  ArmHold,           ///< Arms, constant color
  ArmMix,            ///< Arms, Mixed pattern/color
  ArmFlicker,        ///< Arms, Flickering
  ArmFlickerDouble,  ///< Arms, Flickering twice
  ArmDimming,        ///< Arms, dimming
  ArmFlow,           ///< Arms, flow down the arms
  ArmFlowReverse,    ///< Arms, flow up the arms
  *********************************************************************************************************************************/
