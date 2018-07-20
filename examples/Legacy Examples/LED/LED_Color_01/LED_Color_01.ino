/*****************************************************************
  - LED Color 1
  First way to control the leds
  LedColor(Mode,Color,Time);
    Mode: which leds to control and what pattern (select from first list below)
    Color: what color to set the leds to (select from seconds list below)
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
  CoDrone.LedColor(ArmDimming, Yellow, modeTime);   // Sets the arm leds to yellow dimming every modeTime milliseconds (7)
  delay(delayTime);                                 // wait delayTime (1000)

  CoDrone.LedColor(ArmDimming, Cyan, modeTime);     // Sets the arm leds to Cyan dimming every modeTime milliseconds (7)
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

/***********************************************************************************************************************************
    color options
  AliceBlue, AntiqueWhite, Aqua,  Aquamarine, Azure, Beige,  Bisque, Black, BlanchedAlmond,  Blue, BlueViolet, Brown,
  BurlyWood, CadetBlue, Chartreuse,  Chocolate, Coral, CornflowerBlue,  Cornsilk, Crimson, Cyan, DarkBlue, DarkCyan,
  DarkGoldenRod,  DarkGray, DarkGreen, DarkKhaki,  DarkMagenta, DarkOliveGreen, DarkOrange, DarkOrchid, DarkRed,
  DarkSalmon,  DarkSeaGreen, DarkSlateBlue, DarkSlateGray,  DarkTurquoise, DarkViolet, DeepPink,  DeepSkyBlue, DimGray,
  DodgerBlue, FireBrick, FloralWhite, ForestGreen,  Fuchsia, Gainsboro, GhostWhite, Gold, GoldenRod, Gray, Green,
  GreenYellow, HoneyDew,  HotPink, IndianRed, Indigo, Ivory, Khaki, Lavender,  LavenderBlush, LawnGreen, LemonChiffon,
  LightBlue, LightCoral, LightCyan, LightGoldenRodYellow, LightGray, LightGreen,  LightPink, LightSalmon, LightSeaGreen,
  LightSkyBlue, LightSlateGray, LightSteelBlue,  LightYellow, Lime, LimeGreen, Linen, Magenta, Maroon, MediumAquaMarine,
  MediumBlue, MediumOrchid, MediumPurple, MediumSeaGreen, MediumSlateBlue, MediumSpringGreen, MediumTurquoise, MediumVioletRed,
  MidnightBlue, MintCream, MistyRose, Moccasin, NavajoWhite, Navy, OldLace, Olive, OliveDrab, Orange, OrangeRed, Orchid,
  PaleGoldenRod, PaleGreen, PaleTurquoise, PaleVioletRed, PapayaWhip, PeachPuff, Peru, Pink, Plum, PowderBlue, Purple, RebeccaPurple,
  Red, RosyBrown, RoyalBlue, SaddleBrown, Salmon, SandyBrown, SeaGreen, SeaShell, Sienna, Silver, SkyBlue, SlateBlue, SlateGray,
  Snow, SpringGreen, SteelBlue, Tan, Teal, Thistle, Tomato, Turquoise, Violet, Wheat, White, WhiteSmoke, Yellow, YellowGreen,
  ***********************************************************************************************************************************/
