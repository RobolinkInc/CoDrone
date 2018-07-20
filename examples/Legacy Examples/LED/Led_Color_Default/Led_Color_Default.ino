/*****************************************************************
  - LED Color Default
  Set Default the led for CoDrone
  LedColorDefault(Mode1,Color1[],Time1, Mode2,Color2[],Time2);
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

byte mode1 = ArmHold;                     // Arm LED On 
byte color1[] = {0, 0, 255};              // color1 (R,G,B), blue
byte modeTime1 = 255;                     // How long to wait before changing light patterns(for hold is brightness)

byte mode2 = EyeHold;                     // Eye LED On 
byte color1[] = {0, 0, 255};              // color2 (R,G,B), cyan
byte modeTime1 = 255;                     // How long to wait before changing light patterns(for hold is brightness)

void setup()
{
  CoDrone.begin(115200); // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)

  CoDrone.AutoConnect(NearbyDrone);     // finds and connects to a drone that is nearby

  CoDrone.LedColorDefault(mode1, color1, modeTime2, mode2, color2, modeTime2); 
  // change 2 default color at the same time
}

void loop()
{
  
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
