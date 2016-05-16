/*****************************************************************
  - LED Color 3
  Third way to control the leds
  LedColor(Mode,Color[],Time);
    Mode: which leds to control and what pattern (select from first list below)
    Color[]: what color to set the leds to, it is an array of three bytes, each is
             a value from 0-255 for each color red, blue, and green.
    Time: length of time of each pattern (0-255)
*******************************************************************/
#include <CoDrone.h> // The codrone library that holds all the background files for this

byte modeTime = 7;                        // How long each pattern takes
int delayTime = 1000;                     // How long to wait before changing light patterns

byte color0[] = {255, 0, 0};              //color0 (R,G,B), red
byte color1[] = {0, 255, 0};              //color1 (R,G,B), green
byte color2[] = {0, 0, 255};              //color2 (R,G,B), blue
byte color3[] = {0, 0, 0};                //color3 (R,G,B), black

void setup()
{
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);     // finds and connects to a drone that is nearby
}

void loop()
{
  CoDrone.LedColor(ArmDimming, color0, modeTime);   // set leds to color0, dimming for modeTime time
  delay(delayTime);                                 // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, color1, modeTime);   // set leds to color1, dimming for modeTime time
  delay(delayTime);                                 // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, color2, modeTime);   // set leds to color2, dimming for modeTime time
  delay(delayTime);                                 // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, color3, modeTime);   // set leds to color3, dimming for modeTime time
  delay(delayTime);                                 // wait delayTime (1000)
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
