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
	byte _index = 0;
	
	if(_dType == dType_LedMode)	
	{	
		_len  = 0x03;  
		g = sendInterval;		
	}
	else if(_dType == dType_LedModeColor)		_len  = 0x05; 
	
	//header
	_packet[_index++] = _dType;
	_packet[_index++] = _len;		
	//data
	_packet[_index++] = sendMode;
	_packet[_index++] = r;
	_packet[_index++] = g;  
	_packet[_index++] = b;  
	_packet[_index++] = sendInterval;	
		
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;	
	Send_Processing(_packet,_len,_crc);    
}
//-------------------------------------- LedColor -------------------------------------------------------//
void CoDroneClass::LedColor(byte sendMode, byte sendColor, byte sendInterval)
{		
	LedColorProcess(dType_LedMode, sendMode, sendColor, 0, 0,sendInterval);
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



/*
 *	function	:	setArmRGB(r, g, b)
 *	description : 	setArmRGB(r, g, b) is function for change color for arm LED
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setArmRGB(byte r, byte g, byte b)
{
	byte _packet[9];
	byte _crc[2];

	armred = r;
	armgreen = g;
	armblue = b;

  	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

 	//data
	_packet[2] = armmode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}


/*
 *	function	:	setEyeRGB(r, g, b)
 *	description : 	setEyeRGB(r, g, b) is function for change color for eye LED
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setEyeRGB(byte r, byte g, byte b)
{
	byte _packet[9];
	byte _crc[2];
	
	eyered = r;
	eyegreen = g;
	eyeblue = b;

  	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

 	//data
	_packet[2] = eyemode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);

}

/*
 *	function	:	setAllRGB(r, g, b)
 *	description : 	setAllRGB(r, g, b) is function for change color for both arm and eye LED
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setAllRGB(byte r, byte g, byte b)
{
	setArmRGB(r,g,b);
	setEyeRGB(r,g,b);
}


/*
 *	function	:	setAllRGB(r, g, b)
 *	description : 	setAllRGB(r, g, b) is function for change color for both arm and eye LED
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setArmDefaultRGB(byte r, byte g, byte b)
{
	byte _packet[9];
	byte _crc[2];

	armred = r;
	armgreen = g;
	armblue = b;

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = armmode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}


/*
 *	function	:	setAllRGB(r, g, b)
 *	description : 	setAllRGB(r, g, b) is function for change color for both arm and eye LED
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setEyeDefaultRGB(byte r, byte g, byte b)
{
	byte _packet[9];
	byte _crc[2];

	armred = r;
	armgreen = g;
	armblue = b;

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = eyemode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}


/*
 *	function	:	setAllRGB(r, g, b)
 *	description : 	setAllRGB(r, g, b) is function for change color for both arm and eye LED
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : int (0 ~ 255)
 *					- g : int (0 ~ 255)
 *					- b : int (0 ~ 255)
 *	return 	 	:	none
 */
void CoDroneClass::setAllDefaultRGB(byte r, byte g, byte b)
{
	setArmDefaultRGB(r,g,b);
	setEyeDefaultRGB(r,g,b);
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
	setAllDefaultRGB(255,0,0);
	setAllDefaultMode(LED_HOLD);
}


/*
 *	function	:	setArmMode(mode)
 *	description : 	setArmMode(mode) is function for change mode for arm LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming, LED_Flow,LED_FlowReverse
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setArmMode(byte mode)
{
	byte _packet[9];
	byte _crc[2];

	armmode = mode+0x30;

  	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

 	//data
	_packet[2] = armmode;
	_packet[3] = armred;
	_packet[4] = armgreen;  
	_packet[5] = armblue;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}
/*
 *	function	:	setEyeMode(mode)
 *	description : 	setEyeMode(mode) is function for change mode for eye LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setEyeMode(byte mode)
{
	byte _packet[9];
	byte _crc[2];

	eyemode = mode;

  	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

 	//data
	_packet[2] = eyemode;
	_packet[3] = eyered;
	_packet[4] = eyegreen;  
	_packet[5] = eyeblue;    
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}

/*
 *	function	:	setAllMode(mode)
 *	description : 	setAllMode(mode) is function for change mode for arm and eye LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming, LED_Flow,LED_FlowReverse
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setAllMode(byte mode)
{
	setArmMode(mode);
	setEyeMode(mode);
}


/*
 *	function	:	setArmDefaultMode(mode)
 *	description : 	setArmDefaultMode(mode) is function for change mode for arm LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming, LED_Flow,LED_FlowReverse
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setArmDefaultMode(byte mode)
{
	byte _packet[9];
	byte _crc[2];

	armmode = mode+0x30;

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = armmode;
	_packet[3] = armred;
	_packet[4] = armgreen;  
	_packet[5] = armblue;  
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);
}


/*
 *	function	:	setEyeDefaultMode(mode)
 *	description : 	setEyeDefaultMode(mode) is function for change mode for arm LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setEyeDefaultMode(byte mode)
{
	byte _packet[9];
	byte _crc[2];

	eyemode = mode;

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = eyemode;
	_packet[3] = eyered;
	_packet[4] = eyegreen;  
	_packet[5] = eyeblue;    
	_packet[6] = 100;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
	delay(250);

}


/*
 *	function	:	setAllDefaultMode(mode)
 *	description : 	setAllDefaultMode(mode) is function for change mode for arm and eye LED
 *					Option is LED_None, LED_Hold, LED_Mix, LED_Flicker, 
 *					LED_FlickerDouble, LED_Dimming, LED_Flow,LED_FlowReverse
 *					It will remain after turn off and turn on again
 *	param 		:
 *					- r : mode
 *	return 	 	:	none
 */
void CoDroneClass::setAllDefaultMode(byte mode)
{
	setArmDefaultMode(mode);
	setEyeDefaultMode(mode);
}



