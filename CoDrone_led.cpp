#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>

//-------------------------------------------------------------------------------------------------------//
//----------------------------------------- LED Event ---------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::LedColorProcess(byte _dType, byte sendMode, byte r, byte g, byte b, byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];	
	byte _len  = 0;
	
	if(_dType == dType_LedMode && _dType)	
	{	
		_len  = 0x03;  
		g = sendInterval;		
	}
	else if(_dType == dType_LedModeColor || _dType == dType_LedDefaultColor){
		_len  = 0x05; 
	}		
	
	//header
	_packet[0] = _dType;
	_packet[1] = _len;		
	//data
	_packet[2] = sendMode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = sendInterval;	
		
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;	
	Send_Processing(_packet,_len,_crc);    
	delay(100);
}
void CoDroneClass::LedColor2Process(byte _dType, byte sendMode1, byte r1, byte g1, byte b1, byte sendInterval1, byte sendMode2, byte r2, byte g2, byte b2, byte sendInterval2)
{
	byte _packet[12];
	byte _crc[2];
	byte _len  = 0;
	if(_dType == dType_LedMode2){
		g1 = sendInterval1;
		b1 = sendMode2;
		sendInterval1 = r2;
		sendMode2 = sendInterval2;
		_len=0x6;
	}
	else if(_dType == dType_LedModeColor2 || _dType == dType_LedDefaultColor2){
		_len = 0xa;
	}

	//header
	_packet[0] = _dType;
	_packet[1] = _len;

	//data
	_packet[2] = sendMode1;
	_packet[3] = r1;
	_packet[4] = g1;  
	_packet[5] = b1;  
	_packet[6] = sendInterval1;

	_packet[7] = sendMode2;
	_packet[8] = r2;
	_packet[9] = g2;  
	_packet[10] = b2;  
	_packet[11] = sendInterval2;
	
	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_packet[1],_crc);
	delay(100);     
}
//-------------------------------------- LedColor -------------------------------------------------------//
void CoDroneClass::LedColor(byte sendMode, byte sendColor, byte sendInterval)
{		
	LedColorProcess(dType_LedMode, sendMode, sendColor, 0, 0, sendInterval);
}
void CoDroneClass::LedColor(byte sendMode, byte r, byte g, byte b, byte sendInterval)
{		
	LedColorProcess(dType_LedModeColor, sendMode, r, g, b,sendInterval);
}
void CoDroneClass::LedColor(byte sendMode, byte sendColor[], byte sendInterval)
{
	LedColorProcess(dType_LedModeColor, sendMode,  sendColor[0], sendColor[1], sendColor[2], sendInterval);
}

//----------------------------------- LedColorDefault ---------------------------------------------------//

void CoDroneClass::LedColorDefault(byte sendMode, byte r, byte g, byte b, byte sendInterval)
{	
	LedColorProcess(dType_LedDefaultColor, sendMode, r, g, b,sendInterval);	
}

void CoDroneClass::LedColorDefault(byte sendMode, byte sendColor[], byte sendInterval)
{	
	LedColorProcess(dType_LedDefaultColor, sendMode, sendColor[0], sendColor[1], sendColor[2], sendInterval);	
}

void CoDroneClass::LedColorDefault(byte sendMode, byte sendColor[], byte sendInterval, byte sendMode2, byte sendColor2[], byte sendInterval2)
{	
	byte _packet[12];
	byte _crc[2];

	//header
	_packet[0] = dType_LedDefaultColor2;
	_packet[1] = 10;

	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2];  
	_packet[6] = sendInterval;

	_packet[7] = sendMode2;
	_packet[8] = sendColor2[0];
	_packet[9] = sendColor2[1];  
	_packet[10] = sendColor2[2];  
	_packet[11] = sendInterval2;
	
	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_packet[1],_crc);     
}

//-------------------------------------------------------------------------------------------------------//

/* Parameter descriptions for all functions:
 *
 *  Color: The name of the desired color 

    enum Colors
    {
    AliceBlue, AntiqueWhite, Aqua,
    Aquamarine, Azure, Beige,
    Bisque, Black, BlanchedAlmond,
    Blue, BlueViolet, Brown,
    BurlyWood, CadetBlue, Chartreuse,
    Chocolate, Coral, CornflowerBlue,
    Cornsilk, Crimson, Cyan,
    DarkBlue, DarkCyan, DarkGoldenRod,
    DarkGray, DarkGreen, DarkKhaki,
    DarkMagenta, DarkOliveGreen, DarkOrange,
    DarkOrchid, DarkRed, DarkSalmon,
    DarkSeaGreen, DarkSlateBlue, DarkSlateGray,
    DarkTurquoise, DarkViolet, DeepPink,
    DeepSkyBlue, DimGray, DodgerBlue,
    FireBrick, FloralWhite, ForestGreen,
    Fuchsia, Gainsboro, GhostWhite,
    Gold, GoldenRod, Gray,
    Green, GreenYellow, HoneyDew,
    HotPink, IndianRed, Indigo,
    Ivory, Khaki, Lavender,
    LavenderBlush, LawnGreen, LemonChiffon,
    LightBlue, LightCoral, LightCyan,
    LightGoldenRodYellow, LightGray, LightGreen,
    LightPink, LightSalmon, LightSeaGreen,
    LightSkyBlue, LightSlateGray, LightSteelBlue,
    LightYellow, Lime, LimeGreen,
    Linen, Magenta, Maroon,
    MediumAquaMarine, MediumBlue, MediumOrchid,
    MediumPurple, MediumSeaGreen, MediumSlateBlue,
    MediumSpringGreen, MediumTurquoise, MediumVioletRed,
    MidnightBlue, MintCream, MistyRose,
    Moccasin, NavajoWhite, Navy,
    OldLace, Olive, OliveDrab,
    Orange, OrangeRed, Orchid,
    PaleGoldenRod, PaleGreen, PaleTurquoise,
    PaleVioletRed, PapayaWhip, PeachPuff,
    Peru, Pink, Plum,
    PowderBlue, Purple, RebeccaPurple,
    Red, RosyBrown, RoyalBlue,
    SaddleBrown, Salmon, SandyBrown,
    SeaGreen, SeaShell, Sienna,
    Silver, SkyBlue, SlateBlue,
    SlateGray, Snow, SpringGreen,
    SteelBlue, Tan, Teal,
    Thistle, Tomato, Turquoise,
    Violet, Wheat, White,
    WhiteSmoke, Yellow, YellowGreen,
    EndOfColor
    };


 *	Mode: mode refers to the pattern of the LEDs

 	SOLID  - LED remains on
 	STROBE - LED cycles through all of the colors in Colors
 	BLINK  - LED flashes on and off
 	DOUBLE_BLINK  - LED turns on twice and then turns off for that interval
 	DIMMING - LED fades in and out
 	PULSE   - LED on one arm turns on and off then cycles to the next arm
 	PULSE_REVERSE - same as PULSE but in the opposite direction		
 
 
 *  R, G, B: The red, green, and blue values for the desired color
       R: 0 ~ 255
       G: 0 ~ 255         
       B: 0 ~ 255          
            
 *  Interval: When used with SOLID, interval is the brightness. When
              When used with other modes, interval is flashes per second.
              Value can be 0 ~ 255
              Default value is 100
 *
 *
 */

 
 /*
  * function    : setEyeLed 
  * description : Sets the color, mode, and interval for the eyes
  * param       : Color, Mode, R, G, B, Interval
  * return      : none

 */
void CoDroneClass::setEyeLED(byte Color, byte Mode){
	LedColorProcess(dType_LedMode, Mode, Color, 0, 0, 100);
}

void CoDroneClass::setEyeLED(byte Color, byte Mode, byte Interval){
	LedColorProcess(dType_LedMode, Mode, Color, 0, 0, Interval);
}

void CoDroneClass::setEyeLED(byte R, byte G, byte B, byte Mode){
	LedColorProcess(dType_LedModeColor, Mode, R, G, B, 100);
}

void CoDroneClass::setEyeLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedModeColor, Mode, R, G, B, Interval);
}

/*
 *	function	: setArmLED	
 *	description : Sets the color, mode, and interval for the eyes
 *	param 		: Color, Mode, R, G, B, Interval
 *	return 	 	: none
 */
void CoDroneClass::setArmLED(byte Color, byte Mode){
	LedColorProcess(dType_LedMode, Mode+0x30, Color, 0, 0, 100);
}

void CoDroneClass::setArmLED(byte Color, byte Mode, byte Interval){
	LedColorProcess(dType_LedMode, Mode+0x30, Color, 0, 0, Interval);
}

void CoDroneClass::setArmLED(byte R, byte G, byte B, byte Mode){
	LedColorProcess(dType_LedModeColor, Mode+0x30, R, G, B, 100);
}

void CoDroneClass::setArmLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedModeColor, Mode+0x30, R, G, B, Interval);
}

/*
 *	function	: setAllLED	
 *	description : Sets color, mode, and interval for both the eyes and arms
 *	param 		: Color, Mode, R, G, B, Interval
 *	return 	 	: none
 */
void CoDroneClass::setAllLED(byte Color, byte Mode){
	LedColor2Process(dType_LedMode2, Mode, Color, 0, 0, 100, Mode+0x30, Color, 0, 0, 100);
}

void CoDroneClass::setAllLED(byte Color, byte Mode, byte Interval){
	LedColor2Process(dType_LedMode2, Mode, Color, 0, 0, Interval, Mode+0x30, Color, 0, 0, Interval);
}

void CoDroneClass::setAllLED(byte R, byte G, byte B, byte Mode){
	LedColor2Process(dType_LedModeColor2, Mode, R, G, B, 100, Mode+0x30, R, G, B, 100);
}

void CoDroneClass::setAllLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColor2Process(dType_LedModeColor2, Mode, R, G, B, Interval, Mode+0x30, R, G, B, Interval);
}

// not support yet
// void CoDroneClass::setEyeDefaultLED(byte Color, byte Mode)
// void CoDroneClass::setEyeDefaultLED(byte Color, byte Mode, byte Interval)

/*
 *	function	: setEyeDefaultLED
 *	description : Sets the default color, mode, and interval for the eyes for
 *                when CoDrone is powered on. Settings will remain even after
 *                battery is removed
 *	param 		: R, G, B, Mode, Interval
 *	return 	 	: none
 */
void CoDroneClass::setEyeDefaultLED(byte R, byte G, byte B, byte Mode){
	LedColorProcess(dType_LedDefaultColor, Mode, R, G, B, 100);
}

void CoDroneClass::setEyeDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedDefaultColor, Mode, R, G, B, Interval);
}

// not support yet
// void CoDroneClass::setArmDefaultLED(byte Color, byte Mode)
// void CoDroneClass::setArmDefaultLED(byte Color, byte Mode, byte Interval)

/*
 *	function	: setArmDefaultLED
 *	description : Sets the default color, mode, and interval for the arms for
 *                when CoDrone is powered on. Settings will remain even after
 *                battery is removed
 *	param 		: R, G, B, Mode, Interval
 *	return 	 	: none
 */
void CoDroneClass::setArmDefaultLED(byte R, byte G, byte B, byte Mode){
	LedColorProcess(dType_LedDefaultColor, Mode+0x30, R, G, B, 100);
}

void CoDroneClass::setArmDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedDefaultColor, Mode+0x30, R, G, B, Interval);
}


// not support
// void CoDroneClass::setAllDefaultLED(byte Color, byte Mode)
// void CoDroneClass::setAllDefaultLED(byte Color, byte Mode, byte Interval)

/*
 *	function	: setAllDefaultLED  
 *	description : Sets the default color, mode, and interval for both eyes
 *				  and arms for when CoDrone is powered on. Settings will 
 *				  remain even after battery is removed               
 *	param 		: R, G, B, Mode, Interval
 *	return 	 	: none
 */
void CoDroneClass::setAllDefaultLED(byte R, byte G, byte B, byte Mode){
	LedColor2Process(dType_LedDefaultColor2, Mode, R, G, B, 100, Mode+0x30, R, G, B, 100);
}

void CoDroneClass::setAllDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColor2Process(dType_LedDefaultColor2, Mode, R, G, B, Interval, Mode+0x30, R, G, B, Interval);
}

/*
 *	function	:	resetDefaultLED()
 *	description : 	resetDefaultLED() is function for make LED default color and mode
 *					It will change to red LED is hold
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::resetDefaultLED()
{
	LedColor2Process(dType_LedDefaultColor2, SOLID, 255, 0, 0, 100, SOLID+0x30, 255, 0, 0, 100);	
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyeColor(byte Color, byte brightness){
	LedColorProcess(dType_LedMode, LED_SOLID, Color, 0, 0, brightness);
}

void CoDroneClass::eyeColor(byte R, byte G, byte B, byte brightness){
	LedColorProcess(dType_LedModeColor, LED_SOLID, R, G, B, brightness);
}

void CoDroneClass::armColor(byte Color, byte brightness){
	LedColorProcess(dType_LedMode, LED_SOLID+0x30, Color, 0, 0, brightness);
}

void CoDroneClass::armColor(byte R, byte G, byte B, byte brightness){
	LedColorProcess(dType_LedModeColor, LED_SOLID+0x30, R, G, B, brightness);
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyeDefaultColor(byte R, byte G, byte B, byte Interval){
	LedColorProcess(dType_LedDefaultColor, LED_SOLID, R, G, B, Interval);
}

void CoDroneClass::armDefaultColor(byte R, byte G, byte B, byte Interval){
	LedColorProcess(dType_LedDefaultColor, LED_SOLID+0x30, R, G, B, Interval);
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyePattern(byte Color, byte pattern, byte brightness){
	LedColorProcess(dType_LedMode, pattern, Color, 0, 0, brightness);
}

void CoDroneClass::eyePattern(byte R, byte G, byte B, byte pattern, byte brightness){
	LedColorProcess(dType_LedModeColor, pattern, R, G, B, brightness);
}

void CoDroneClass::armPattern(byte Color, byte pattern, byte brightness){
	LedColorProcess(dType_LedMode, pattern+0x30, Color, 0, 0, brightness);
}

void CoDroneClass::armPattern(byte R, byte G, byte B, byte pattern, byte brightness){
	LedColorProcess(dType_LedModeColor, pattern+0x30, R, G, B, brightness);
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyeDefaultPattern(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedDefaultColor, Mode, R, G, B, Interval);
}

void CoDroneClass::armDefaultPattern(byte R, byte G, byte B, byte Mode, byte Interval){
	LedColorProcess(dType_LedDefaultColor, Mode+0x30, R, G, B, Interval);
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyeStrobe(){
	LedColorProcess(dType_LedMode, LED_STROBE, 0, 0, 0, 100);
}

void CoDroneClass::armStrobe(){
	LedColorProcess(dType_LedModeColor, LED_STROBE+0x30, 0, 0, 0, 100);
}

//------------------------------------------------------------------------------------------------

void CoDroneClass::eyeOff(){
	LedColorProcess(dType_LedMode, LED_NONE, 0, 0, 0, 0);
}

void CoDroneClass::armOff(){
	LedColorProcess(dType_LedModeColor, LED_NONE+0x30, 0, 0, 0, 0);
}


