/*
  CoDrone.cpp - CoDrone library
  Copyright (C) 2014 RoboLink.  All rights reserved.
  LastUpdate : 2016-04-20
*/

#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>
/***************************************************************************/

/***************************************************************************/

static const unsigned short crc16tab[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};



unsigned short CoDroneClass::CRC16_Make(unsigned char *buf, int len) //CRC16-CCITT Format
{
	unsigned short crc = 0 ;
	for (int counter = 0; counter < len; counter++)
	{
	crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ * (char *)buf++) & 0x00FF];
	}
	return crc;
}

boolean CoDroneClass::CRC16_Check(unsigned char data[], int len, unsigned char crc[])
{
	boolean crcCheck = false;
	
	unsigned short receiveCRC = ((crc[1] << 8) | (crc[0]  & 0xff));
	unsigned short  makeCRC = CRC16_Make(data, len + 2);
	
	if (receiveCRC == makeCRC )	   	   	crcCheck = true;
	else					  								  	crcCheck = false;
	return crcCheck;
}

/***************************************************************************/

void CoDroneClass::begin(long baud)
{
	DRONE_SERIAL.begin(baud);   		// 드론과 통신 개시	(115200bps)
				
	#if defined(FIND_HWSERIAL1)
		DEBUG_SERIAL.begin(baud);		// Serial Debug Begin	(115200bps)
		debugMode = 1;					// DEBUG MODE 0 = OFF, 1 = ON
		displayMode = 0;				// LED Display 0 = BOARD LED 0FF, 1 = BOARD LED ON	
	#endif
				
	SendInterval = 50; 					// millis seconds		
	
	analogOffset = 10;					// analog sensor offset
	
	LED_Start();

	// Connected Drone Address Read
	if (EEPROM.read(EEP_AddressCheck) == 1)
	{  	
		for (int i = 0; i <= 5; i++)
		{
			devAddressConnected[i] = EEPROM.read(EEP_AddressFirst+i);
		}		
	}		
  delay(500);
  
  //Link Active Mode
  Send_LinkModeBroadcast(LinkModeActive);		
  
  delay(500);  
}

/***************************************************************************/
//////////////////////Command////////////////////////////////////////////
/***************************************************************************/

////////////////////link board////////////////////////

void CoDroneClass::Send_LinkModeBroadcast(byte mode)
{
	Send_Command(cType_LinkModeBroadcast, mode);
}

void CoDroneClass::Send_LinkState()
{
	Send_Command(cType_Request, dType_LinkState);
}

void CoDroneClass::LinkReset()
{
	Send_Command(cType_LinkSystemReset, 0);
}

void CoDroneClass::Send_Discover(byte action)
{	
	if(action == DiscoverStop)	  	Send_Command(cType_LinkDiscoverStop, 0);	//DiscoverStop
	else if(action == DiscoverStart)	
	{
		Send_Command(cType_LinkDiscoverStart, 0);  	//DiscoverStart  
		discoverFlag = 1;
	}
}

void CoDroneClass::Send_Connect(byte index) //index 0, 1, 2
{
	connectFlag = 1;
	
	if(index == 0)
	{				
		for (int i = 0; i <= 5; i++)		devAddressBuf[i] = devAddress0[i];
	}	
	else if (index == 1)
	{
		for (int i = 0; i <= 5; i++)		devAddressBuf[i] = devAddress1[i];
	}
	else if (index == 2)
	{
		for (int i = 0; i <= 5; i++)		devAddressBuf[i] = devAddress2[i];
	}	
				
	else if (index == 3)
	{
		for (int i = 0; i <= 5; i++)		devAddressBuf[i] = devAddress3[i];
	}	
		
	else if (index == 4)
	{
		for (int i = 0; i <= 5; i++)		devAddressBuf[i] = devAddress4[i];
	}	
		
		
	 #if defined(FIND_HWSERIAL1)	 
	                  
	 if(debugMode == 1)
	 {								  	
	     DEBUG_SERIAL.println(index, HEX);			    
	 }
	 
	 #endif  
		
		
				
	Send_Command(cType_LinkConnect, index);
}
void CoDroneClass::Send_Disconnect()
{
	Send_Command(cType_LinkDisconnect, 0);
}

void CoDroneClass::Send_RSSI_Polling(byte action)
{
	if(action == cType_LinkRssiPollingStop)	  		Send_Command(cType_LinkRssiPollingStop, 0);			//RssiPollingStop
	else if(action == cType_LinkRssiPollingStart)	Send_Command(cType_LinkRssiPollingStart, 0x02);  	//RssiPollingStart	(0x02 * 100 = 200ms)
}

//////////////////ModeDrone/////////////////////////////

void CoDroneClass::DroneModeChange(byte event)
{
	sendCheckFlag = 1;
	Send_Command(cType_ModeDrone, event);
	delay(300);
}

void CoDroneClass::Send_DroneMode(byte event)
{
	Send_Command(cType_ModeDrone, event);
}
////////////////////FlightEvent////////////////////////

void CoDroneClass::Send_Coordinate(byte mode)
{
	if(mode == cSet_Absolute)	  			Send_Command(cType_Coordinate, cSet_Absolute);
	else if(mode == cSet_Relative) 			Send_Command(cType_Coordinate, cSet_Relative);
}
void CoDroneClass::Set_Trim(byte event)
{
	sendCheckFlag = 1;
	Send_Command(cType_Trim, event);
}

void CoDroneClass::Send_ClearGyroBiasAndTrim()
{
	sendCheckFlag = 1;
	Send_Command(cType_ClearGyroBiasAndTrim, 0);
}

void CoDroneClass::FlightEvent(byte event)
{
	sendCheckFlag = 1;
	Send_Command(cType_FlightEvent, event);
}

void CoDroneClass::DriveEvent(byte event)
{
	Send_Command(cType_DriveEvent, event);
}

void CoDroneClass::Send_ResetHeading()
{
	Send_Command(cType_ResetHeading, 0);
}

/***************************************************************************/
//////////////////////////MOTOR///////////////////////////////////////////

void CoDroneClass::Set_Motor()
{
	sendCheckFlag = 1;
	
	byte _packet[18];
	byte _crc[2];
	
	byte _cType = dType_Motor;
	byte _len   = 16;  
	
	/*
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	       
	//data 
	//1
	_packet[2] = 0xbe;
	_packet[3] = 0x0e;  
	
	_packet[4] = 0;
	_packet[5] = 0;
	
	//2
	_packet[6] = 0xe1;
	_packet[7] = 0x0a; 
	
	_packet[8] = 0;
	_packet[9] = 0;
	
	//3
	_packet[10] = 0xff;
	_packet[11] = 0x0f;
	
	_packet[12] = 0;
	_packet[13] = 0;
	
	//4
	_packet[14] = 0x40;
	_packet[15] = 0x0f;
	
	_packet[16] = 0;
	_packet[17] = 0;
	*/
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc); 
	    
	////////////////////////////////////////////
	//Send_Check(_packet,_len,_crc);
}


/***************************************************************************/
//////////////////////////TRIM///////////////////////////////////////////

void CoDroneClass::Set_TrimAll(int _roll, int _pitch, int _yaw, int _throttle, int _wheel)
{
	sendCheckFlag = 1;
	
	byte _packet[12];
  	byte _crc[2];
  
  	byte _cType = dType_TrimAll;
  	byte _len   = 10;  
  
  	//header
 	_packet[0] = _cType;
  	_packet[1] = _len;
    
	byte L_roll 		= _roll & 0xff;
	byte H_roll 		= (_roll >> 8) & 0xff;

	byte L_pitch 		= _pitch & 0xff;
	byte H_pitch 		= (_pitch >> 8) & 0xff;
	
	byte L_yaw	 		= _yaw & 0xff;
	byte H_yaw 			= (_yaw >> 8) & 0xff;
	
	byte L_throttle 	= _throttle & 0xff;
	byte H_throttle 	= (_throttle >> 8) & 0xff;
	
	byte L_wheel 		= _wheel & 0xff;
	byte H_wheel 		= (_wheel >> 8) & 0xff;
	
 	//data
	_packet[2] = L_roll;
	_packet[3] = H_roll;
	
	_packet[4] = L_pitch;
	_packet[5] = H_pitch;
	
	_packet[6] = L_yaw;
	_packet[7] = H_yaw;
	
	_packet[8] = L_throttle;
	_packet[9] = H_throttle;
	
	_packet[10] = L_wheel;
	_packet[11] = H_wheel;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);
	
	////////////////////////////////////////////
	Send_Check(_packet,_len,_crc);
	////////////////////////////////////////////	
}

void CoDroneClass::Set_TrimFlight(int _roll, int _pitch, int _yaw, int _throttle)
{
	sendCheckFlag = 1;
	
	byte _packet[10];
	byte _crc[2];
	
	byte _cType = dType_TrimFlight;
	byte _len   = 8;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	byte L_roll 		= _roll & 0xff;
	byte H_roll 		= (_roll >> 8) & 0xff;
	
	byte L_pitch 		= _pitch & 0xff;
	byte H_pitch 		= (_pitch >> 8) & 0xff;
	
	byte L_yaw	 		= _yaw & 0xff;
	byte H_yaw 			= (_yaw >> 8) & 0xff;
	
	byte L_throttle = _throttle & 0xff;
	byte H_throttle = (_throttle >> 8) & 0xff;
	
	//data
	_packet[2] = L_roll;
	_packet[3] = H_roll;
	
	_packet[4] = L_pitch;
	_packet[5] = H_pitch;
	
	_packet[6] = L_yaw;
	_packet[7] = H_yaw;
	
	_packet[8] = L_throttle;
	_packet[9] = H_throttle;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc); 		
}

void CoDroneClass::Set_TrimDrive(int _wheel)
{
	sendCheckFlag = 1;
	
	byte _packet[4];
	byte _crc[2];
	
	byte _cType = dType_TrimDrive;
	byte _len   = 2;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	byte L_wheel 		= _wheel & 0xff;
	byte H_wheel 		= (_wheel >> 8) & 0xff;
	
	//data
	_packet[2] = L_wheel;
	_packet[3] = H_wheel;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc); 
		
	////////////////////////////////////////////
	Send_Check(_packet,_len,_crc);	
	///////////////////////////////////////////
}

void CoDroneClass::Set_TrimReset()
{
	Set_TrimAll(0,0,0,0,0);	
}

/***************************************************************************/
void CoDroneClass::Send_Check(byte _data[], byte _length, byte _crc[])
{
	if(sendCheckFlag == 1)
	{
		timeOutSendPreviousMillis = millis();
		
		while(sendCheckFlag != 3)
		{
			while(!TimeOutSendCheck(SEND_CHECK_TIME))
			{
				Receive();
				if(sendCheckFlag == 3) break;
			}
			if(sendCheckFlag == 3) break;
			if(sendCheckCount == 3) break;			//check count 
			Send_Processing(_data,_length,_crc);
			sendCheckCount++;
		}
		sendCheckFlag = 0;
		sendCheckCount = 0;
	}	
}
/***************************************************************************/
///////////////////////CONTROL///////////////////////////////////////////////
/***************************************************************************/
void CoDroneClass::Control(int interval)
{
    if (TimeCheck(interval))  //delay
    {
		Control();
		PreviousMillis = millis();
    }
}

void CoDroneClass::Control()
{	
	byte _packet[10];
	byte _crc[2];
	
	byte _cType = dType_Control;
	byte _len = 4;
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = roll;
	_packet[3] = pitch;
	_packet[4] = yaw;
	_packet[5] = throttle;
	//
	
	prevControl[0] = roll;
	prevControl[1] = pitch;
	prevControl[2] = yaw;
	prevControl[3] = throttle;
		
		
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);  
	
	roll = 0;
	pitch = 0;
	yaw = 0;
	throttle = 0;
	
	////////////////////////////////////////////	
	sendCheckFlag = 0;	
	Send_Check(_packet,_len,_crc);
	////////////////////////////////////////////
}

////////////////////////////////////////////////////////

void CoDroneClass::Send_Command(int sendCommand, int sendOption)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_Command;
	byte _len   = 0x02;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendCommand;
	_packet[3] = sendOption;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);
			    
	////////////////////////////////////////////			
	Send_Check(_packet,_len,_crc);	
	////////////////////////////////////////////
}

////////////////////////////////////////////

void CoDroneClass::BattleBegin(byte teamSelect)
{
	team = teamSelect;
	
	if(team == TEAM_RED)
	{		
		weapon = RED_MISSILE;
			
		CoDrone.LedColor(ArmHold,  Red, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, Red, 7);	  		    
	}	
	
	else if	(team == TEAM_BLUE)
	{		
		weapon = BLUE_MISSILE;
		
		CoDrone.LedColor(ArmHold,  Blue, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, Blue, 7);	  
	}	
	
	else if	(team == TEAM_GREEN)
	{			
		weapon = GREEN_MISSILE;
		
		CoDrone.LedColor(ArmHold,  Green, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, Green, 7);
	}
	
	else if	(team == TEAM_YELLOW)
	{		
		weapon = YELLOW_MISSILE;
		
		CoDrone.LedColor(ArmHold,  Yellow, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, Yellow, 7);
	}
	else if (team == FREE_PLAY)
	{		
		CoDrone.LedColor(ArmHold,  White, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, White, 7);
				
		weapon = FREE_MISSILE;				
	}
	
	delay (300);
}


void CoDroneClass::BattleReceive()
{
	Receive();
	
	if(irMassageReceive > 0)
	{		
		if(team == TEAM_RED)
		{
			if(irMassageReceive == BLUE_MISSILE || irMassageReceive == GREEN_MISSILE || irMassageReceive == YELLOW_MISSILE || irMassageReceive == FREE_MISSILE)
			{
				BattleDamageProcess();		
			}			
		}				
			
		else if(team == TEAM_BLUE)
		{
			if(irMassageReceive == RED_MISSILE || irMassageReceive == GREEN_MISSILE || irMassageReceive == YELLOW_MISSILE || irMassageReceive == FREE_MISSILE)
			{				
				BattleDamageProcess();
			}			
		}					
		
		else if(team == TEAM_GREEN)
		{
			if(irMassageReceive == BLUE_MISSILE || irMassageReceive == RED_MISSILE || irMassageReceive == YELLOW_MISSILE || irMassageReceive == FREE_MISSILE)
			{
				BattleDamageProcess();				
			}			
		}		
						
		else if(team == TEAM_YELLOW)
		{
			if(irMassageReceive == BLUE_MISSILE || irMassageReceive == GREEN_MISSILE || irMassageReceive == RED_MISSILE || irMassageReceive == FREE_MISSILE)
			{				
				BattleDamageProcess();
			}			
		}		
			
		else if(team == FREE_PLAY)
		{
			if(irMassageReceive == RED_MISSILE || irMassageReceive == BLUE_MISSILE || irMassageReceive == GREEN_MISSILE || irMassageReceive == YELLOW_MISSILE || irMassageReceive == FREE_PLAY)
			{				
				BattleDamageProcess();
			}			
		}		
		
		irMassageReceive = 0;
	}	
}

void CoDroneClass::BattleDamageProcess()
{
	if(displayMode == 1)
	{		
		energy--;
		
		if(energy > 0)
		{
		DDRC = 0xff;					
		PORTC = (0xff >> (MAX_ENERGY - energy)) << ((MAX_ENERGY - energy) / 2);
		 
		CoDrone.Buzz(4000, 8);			
	}
	
	else
	{			
		delay(300);
		
		CoDrone.LedColor(ArmNone, Black, 7);
		DDRC = 0xff;			
		PORTC = 0x00;
		CoDrone.Buzz(3000, 4);
		delay(100);
		CoDrone.Buzz(2000, 4);
		delay(100);
		CoDrone.Buzz(3000, 4);
		delay(100);
		CoDrone.Buzz(2000, 4);
		delay(100);
		CoDrone.Buzz(3000, 4);
		delay(100);
		CoDrone.Buzz(2000, 4);
		delay(100);
		CoDrone.Buzz(3000, 4);
		delay(100);
		CoDrone.Buzz(2000, 4);		  		  
	}
	
	delay(100);
	DDRC = 0b01100110;
	PORTC = 0b00100100;
	}
}



void CoDroneClass::BattleShooting()
{
	sendCheckFlag = 1;
		
	byte _packet[12];		 
	byte _crc[2];
	
	byte _cType = dType_LedEventCommandIr;
	byte _len   = 10;
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	//led event base
	_packet[2] = ArmDimming;
	_packet[3] = Magenta;
	_packet[4] = 7;
	_packet[5] = 2;
	
	//command base
	_packet[6] = cType_FlightEvent;
	_packet[7] = fEvent_Shot;
	
	//irData u32 - 4byte
	unsigned long data = weapon;
	_packet[8] = data & 0xff;
	_packet[9] = (data >> 8) & 0xff;
	_packet[10] 	=	(data >> 16) & 0xff;
	_packet[11] 	= (data >> 24) & 0xff;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);  
	
	Send_Check(_packet,_len,_crc);
}



void CoDroneClass::LedColor(byte sendMode, byte sendColor, byte sendInterval)
{	
	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedMode;
	byte _len   = 0x03;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor;
	_packet[4] = sendInterval;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);     
}


void CoDroneClass::LedColor(byte sendMode, byte r, byte g, byte b, byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedModeColor;
	byte _len   = 0x05;  
	
	//header
	_packet[0] = _cType;
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
}

void CoDroneClass::LedColor(byte sendMode, byte sendColor[], byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedModeColor;
	byte _len   = 0x05;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2];  
	_packet[6] = sendInterval;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);     
}


void CoDroneClass::LedEvent(byte sendMode, byte sendColor, byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedEvent;
	byte _len   = 0x04;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor;
	_packet[4] = sendInterval;
	_packet[5] = sendRepeat;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);     
}

void CoDroneClass::LedEvent(byte sendMode, byte sendColor[], byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedEventColor;
	byte _len   = 0x06;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2];  
	_packet[6] = sendInterval;
	_packet[7] = sendRepeat;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);     
}


void CoDroneClass::LedEvent(byte sendMode, byte r, byte g, byte b, byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];
	
	byte _cType = dType_LedEventColor;
	byte _len   = 0x06;  
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	//data
	_packet[2] = sendMode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b; 
	_packet[6] = sendInterval;
	_packet[7] = sendRepeat;
	
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);     
}


//////////////////Request//////////////////////////////////////

void CoDroneClass::Request_DroneState()
{
	Send_Command(cType_Request, Req_State);    
}
void CoDroneClass::Request_DroneAttitude()
{
	sendCheckFlag = 1;
	Send_Command(cType_Request, Req_Attitude);    
}
void CoDroneClass::Request_DroneGyroBias()
{
	Send_Command(cType_Request, Req_GyroBias);    
}
void CoDroneClass::Request_TrimAll()
{
	sendCheckFlag = 1;
	Send_Command(cType_Request, Req_TrimAll);    
}
void CoDroneClass::Request_TrimFlight()
{
	Send_Command(cType_Request, Req_TrimFlight);    
}
void CoDroneClass::Request_TrimDrive()
{
	Send_Command(cType_Request, Req_TrimDrive);    
}
void CoDroneClass::Request_ImuRawAndAngle()
{
	Send_Command(cType_Request, Req_ImuRawAndAngle);    
}
void CoDroneClass::Request_Pressure()
{
	Send_Command(cType_Request, Req_Pressure);    
}
void CoDroneClass::Request_ImageFlow()
{
	Send_Command(cType_Request, Req_ImageFlow);    
}
void CoDroneClass::Request_Button()
{
	Send_Command(cType_Request, Req_Button);    
}
void CoDroneClass::Request_Battery()
{
	Send_Command(cType_Request, Req_Batery);    
}
void CoDroneClass::Request_Motor()
{
	Send_Command(cType_Request, Req_Motor);    
}
void CoDroneClass::Request_Temperature()
{
	Send_Command(cType_Request, Req_Temperature);    
}

void CoDroneClass::Send_Ping()
{   
	byte _packet[10];
	byte _crc[2];
	
	byte _cType = dType_Ping;
	byte _len = 0;
	
	//header
	_packet[0] = _cType;
	_packet[1] = _len;
	
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	
	Send_Processing(_packet,_len,_crc);      
}

/////////////////////////////////////////////////////////////

void CoDroneClass::AutoConnect(byte mode)
{		
	// Connected check
	LinkStateCheck();		
	if (linkState  == linkMode_Connected)
	{
		pairing = true;
		LED_Connect();    
	}
  
  // AutoConnect start
	else     
	{				
	  if (mode == NearbyDrone)	
	  {
	  	Send_Discover(DiscoverStart);  
	  	PreviousMillis = millis();
	  		  	
		LED_DDRC(0xff);
			
	  	while(!pairing)
	  	{  			  		
	  		 if((discoverFlag == 3) && (connectFlag == 0)) // Address find
	  		 {	  		  	
	  		  	LED_Standard();	  
	  		  	
	  		  	PreviousMillis = millis();	
	  		  	while(!TimeCheck(1000))			
	  			{  
	  		  	   Receive();  
	  		  	}	 		  
	  		  	 		  		 	
	  		 	delay(50);
	  		 	discoverFlag = 0;
	  		 	Send_ConnectNearbyDrone();  	  		 	// Connect Start
	  		 }
	  		 
	  		 /*
	  		 else if (discoverFlag == 4)					// Address not find : re-try
	  		 {
	  		 	delay(50);
	  		 	Send_Discover(DiscoverStart);
	  		 	PreviousMillis = millis();
	  		 }
	  		 */
	  		 
	  		 else
	  		 {	  	
		  		if (TimeCheck(400))							// time out & LED
	    		{
		      		if (displayLED++ == 4) 
		      		{
		      			displayLED = 0;	 
		      			delay(50);     
		      			Send_Discover(DiscoverStart);
		      		}
		      		
			      	LED_Move_Radar(displayLED);			      	
			      	PreviousMillis = millis();   		     
				}
	  		}	
	  		  	
	 
		  Receive();  
	  }
	  	delay(50);  	 	  	
	  }
	  
	  else if(mode == ConnectedDrone)   
	  {
	  	Send_Discover(DiscoverStart);  
	  	PreviousMillis = millis();
	  	
		LED_DDRC(0xff);
			
	  	while(!pairing)
	  	{  		
	  		
  		 if ((discoverFlag == 3) && (connectFlag == 0))	// Address find
  		 {  	  		 	
  		 	LED_Standard();
  		 	  		 		 

  		  	PreviousMillis = millis();	
  		  	while(!TimeCheck(1000))			
  			{  
  		  	   Receive();  
  		  	}	 		   		 
  		 	  		 		  	
  		 	delay(50);
  		 	discoverFlag = 0;
  		 	Send_ConnectConnectedDrone();  	 	// Connect Start 		 	
  		 }
  		 /*
  		 else if (discoverFlag == 4)			// Address not find : re-try
  		 {
  		 	Send_Discover(DiscoverStart);
  		  	PreviousMillis = millis();
  		 }
  		 */
  		 else
  		 {	  	
	  		if (TimeCheck(400))  // time out & LED
    		{
	      		if (displayLED++ == 4) 
	      		{
	      			displayLED = 0;	 
	      			delay(50);     
	      			Send_Discover(DiscoverStart);
	      		}      		
	      		LED_Move_Radar(displayLED);
	     
	      		PreviousMillis = millis();   		     
			}
  		}
  		
		Receive();  		
	  	}
	  	delay(50);
	  } 
	}
}

void CoDroneClass::AutoConnect(byte mode, byte address[])
{		
	// Connected check
	LinkStateCheck();		
	if (linkState  == linkMode_Connected)
	{
		pairing = true;
		LED_Connect();
	}
    
  // AutoConnect start
	else     
	{		 	
	  if (mode == AddressInputDrone)		
	  {
	  	Send_Discover(DiscoverStart);  
	  	PreviousMillis = millis();
	  	
	  	LED_DDRC(0xff);

	  	while(!pairing)
	  	{  			  		 	  		   		 
	  		 if((discoverFlag == 3) && (connectFlag == 0))	//Address find
	  		 {  		  		 	
	  		 	LED_Standard();
	  		 	
	  		  	PreviousMillis = millis();	
	  		  	while(!TimeCheck(1000))			
	  			{  
	  		  	   Receive();  
	  		  	}	 		   	
	  		  	
	  		 	delay(50);
	  		 	discoverFlag = 0;
	  		 	Send_ConnectAddressInputDrone(address);  	  		//  Connect Start 			 	
	  		 }  	
	  		 /*	
	  		 else if (discoverFlag == 4)	// Address not find : re-try
	  		 {
	  		 	Send_Discover(DiscoverStart);
	  		 	PreviousMillis = millis();
	  		 }
	  		 */
	  		 
	  		 else
	  		 {	  	
		  		if (TimeCheck(400))	//time out & LED
	    		{
	      		if (displayLED++ == 4) 
	      		{
	      			displayLED = 0;	 
	      			delay(50);     
	      			Send_Discover(DiscoverStart);
	      		}
	      		LED_Move_Radar(displayLED);
	      
	      		PreviousMillis = millis();   		     
				}
	  		}
	  		 Receive();  			
	  	}
	  	delay(50);  	
	  }
	}
}


void CoDroneClass::Send_ConnectAddressInputDrone(byte address[])
{
  if (devCount > 0)
  {
		//ConnectedDrone same address check
		byte AddrCheck0 = 0;
		byte AddrCheck1 = 0;
		byte AddrCheck2 = 0;
		byte AddrCheck3 = 0;
		byte AddrCheck4 = 0;
		
		for (int i = 0; i <= 5; i++)
		{
			if (address[i] == devAddress0[i])  AddrCheck0++;
			if (address[i] == devAddress1[i])  AddrCheck1++;
			if (address[i] == devAddress2[i])  AddrCheck2++;
			if (address[i] == devAddress3[i])  AddrCheck3++;		
			if (address[i] == devAddress4[i])  AddrCheck4++;
			
		}		
	  if(AddrCheck0 == 6)	Send_Connect(0);   
	  else if(AddrCheck1 == 6)	Send_Connect(1);   
	  else if(AddrCheck2 == 6)	Send_Connect(2);    
	  else if(AddrCheck3 == 6)	Send_Connect(3);    
	  else if(AddrCheck4 == 6)	Send_Connect(4);    
	  	  	  
	}	
}

void CoDroneClass::Send_ConnectConnectedDrone()
{	
  if (devCount > 0)
  {
		//ConnectedDrone same address check
		byte AddrCheck0 = 0;
		byte AddrCheck1 = 0;
		byte AddrCheck2 = 0;
		byte AddrCheck3 = 0;
		byte AddrCheck4 = 0;
				
		for (int i = 0; i <= 5; i++)
		{
			if (devAddressConnected[i] == devAddress0[i])  AddrCheck0++;
			if (devAddressConnected[i] == devAddress1[i])  AddrCheck1++;
			if (devAddressConnected[i] == devAddress2[i])  AddrCheck2++;
			if (devAddressConnected[i] == devAddress3[i])  AddrCheck3++;
			if (devAddressConnected[i] == devAddress4[i])  AddrCheck4++;			
		}		
		
		if(AddrCheck0 == 6)			Send_Connect(0);   
		else if(AddrCheck1 == 6)	Send_Connect(1);   
		else if(AddrCheck2 == 6)	Send_Connect(2);    
		else if(AddrCheck3 == 6)	Send_Connect(3);   
		else if(AddrCheck4 == 6)	Send_Connect(4);   	  
	}	
}

void CoDroneClass::Send_ConnectNearbyDrone()
{

  byte highRSSIDrone = 0;	
  int _address[] = {devRSSI0, devRSSI1, devRSSI2, devRSSI3, devRSSI4};
		
  if (devCount > 0)
  {  	
  	
   for (int i = 0 ; i < 5 ; i++)
  {
    if (_address[highRSSIDrone] > _address[i]) highRSSIDrone = highRSSIDrone;
    else highRSSIDrone = i;
  }

  Send_Connect(highRSSIDrone);
  	
  }
}


void CoDroneClass::Send_Processing(byte _data[], byte _length, byte _crc[])
{		
  byte _packet[30];
  
  //START CODE  
  _packet[0] = START1;
  _packet[1] = START2;

  //HEADER & DATA
  for(int i = 0; i < _length + 3 ; i++)
  {
   _packet[i+2] = _data[i];	   
  }
  //CRC  
  _packet[_length + 4] =_crc[1];
  _packet[_length + 5] =_crc[0]; 
    
 	DRONE_SERIAL.write(_packet, _length + 6);
 	 	 
  #if defined(FIND_HWSERIAL1)
	if(debugMode == 1)
	{
		DEBUG_SERIAL.print("> SEND : ");
		
		for(int i = 0; i < _length+6 ; i++)
		{
	  	DEBUG_SERIAL.print(_packet[i],HEX);	  	
	  	DEBUG_SERIAL.print(" ");	     
	  }
	  DEBUG_SERIAL.println("");	
	}	
  #endif 	
}

/***************************************************************************/

void CoDroneClass::Receive()
{	
	if (DRONE_SERIAL.available() > 0)
  	{
    int input = DRONE_SERIAL.read();
    
    #if defined(FIND_HWSERIAL1)
	if(debugMode == 1)
	{
	//
	// DEBUG_SERIAL.print(input,HEX);	
	//
	}	
    #endif
    
    cmdBuff[cmdIndex++] = (char)input;

    if (cmdIndex >= MAX_PACKET_LENGTH)
    {
      checkHeader = 0;
      cmdIndex = 0;
    }
    else
    {
      if (cmdIndex == 1)
      {
        if (cmdBuff[0] == START1)	checkHeader = 1;
        else
        {
          checkHeader = 0;
          cmdIndex = 0;
        }
      }
      else if (cmdIndex == 2)
      {
        if (checkHeader == 1)
        {
          if (cmdBuff[1] == START2)	checkHeader = 2;
          else
          {
            checkHeader = 0;
            cmdIndex = 0;
          }
        }
      }      
      else if (checkHeader == 2)
      {
        if (cmdIndex == 3)
        {
          receiveDtype =  cmdBuff[2];
        }
        else if (receiveDtype != dType_StringMessage) //not message string
        {
          if (cmdIndex == 4)
          {
            receiveLength = cmdBuff[3];
          }
          else if (cmdIndex > 4)
          {        
            if (receiveLength + 6 <= cmdIndex)
            {            	
            	byte crcBuff[2];
            	crcBuff[0]  = cmdBuff[cmdIndex - 2];
              crcBuff[1]  = cmdBuff[cmdIndex - 1];

			byte dataBuff[30];

			for(int i = 0; i < receiveLength + 5 ; i++)	dataBuff[i - 3] = cmdBuff[i - 1];   

              if (CRC16_Check(dataBuff, receiveLength, crcBuff))  receiveComplete = 1;
              else  receiveComplete = -1;

              byte receiveCompleteData[16];

              if (receiveComplete == 1)
              {                       	              	
              	if (receiveDtype == dType_LinkState)		
                {
                	receiveLinkState = dataBuff[2];
                	receiveLikMode = dataBuff[3];                	
                }                                	
                else if (receiveDtype == dType_LinkEvent)		
                {
                	receiveEventState = dataBuff[2];
                }
                          
                /***********************************************/     
                            
                else if (receiveDtype ==  dType_Button)//dron Button
                {
                	receiveCompleteData[0] = dataBuff[2];
                }
                          
             		else if (receiveDtype == dType_TrimDrive)		//dron TrimDrive
                { 
                	receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];	               				                
                }   
                                                                     
                else if (receiveDtype == dType_IrMessage)		//IrMessage
                {           	
	                receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];	              
              	}            
              	              
                else if ((receiveDtype == dType_Attitude) || (receiveDtype == dType_GyroBias))		//dron Attitude	|| dron GyroBias
                { 
                	receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];
	                receiveCompleteData[5] = dataBuff[7];	  	                	                	      				                
                }         
                else if (receiveDtype == dType_State)				//dron state
                {           	
	                receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];
	                receiveCompleteData[5] = dataBuff[7];	
	                receiveCompleteData[6] = dataBuff[8];	  	              
              	}
                          
                else if ((receiveDtype == dType_TrimFlight)|| (receiveDtype ==  dType_ImageFlow) || (receiveDtype ==  dType_Temperature))	//dron TrimFlight  || dron ImageFlow || dron Temperature
                { 
                	receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];
	                receiveCompleteData[5] = dataBuff[7];	                
	                receiveCompleteData[6] = dataBuff[8];
	                receiveCompleteData[7] = dataBuff[9];            				                
                }                    
                    
                else if (receiveDtype == dType_ImuRawAndAngle)	//dron ImuRawAndAngle
                {
                	receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];
	                receiveCompleteData[5] = dataBuff[7];	  	 
	              	receiveCompleteData[6] = dataBuff[8];	  	  
	               	receiveCompleteData[7] = dataBuff[9];	  	 
	                receiveCompleteData[8] = dataBuff[10];	  		                                   
                }
                                        
                else if (receiveDtype == dType_TrimAll)				//dron TrimAll
                { 
                	receiveCompleteData[0] = dataBuff[2];
	                receiveCompleteData[1] = dataBuff[3];
	                receiveCompleteData[2] = dataBuff[4];
	                receiveCompleteData[3] = dataBuff[5];
	                receiveCompleteData[4] = dataBuff[6];
	                receiveCompleteData[5] = dataBuff[7];	                
	                receiveCompleteData[6] = dataBuff[8];
	                receiveCompleteData[7] = dataBuff[9];
	                receiveCompleteData[8] = dataBuff[10];
	                receiveCompleteData[9] = dataBuff[11];	                    				                
                }           
                                     
                else if ((receiveDtype == dType_Pressure) || (receiveDtype ==  dType_Batery) ||  (receiveDtype ==  dType_Motor))	//dron Pressure || dron Batery || dron Motor
                {
                	receiveCompleteData[0] = dataBuff[2];
                	receiveCompleteData[1] = dataBuff[3];	
                	receiveCompleteData[2] = dataBuff[4];	
                	receiveCompleteData[3] = dataBuff[5];
                	receiveCompleteData[4] = dataBuff[6];
                	receiveCompleteData[5] = dataBuff[7];
                	receiveCompleteData[6] = dataBuff[8];
                	receiveCompleteData[7] = dataBuff[9];
                	receiveCompleteData[8] = dataBuff[10];
                	receiveCompleteData[9] = dataBuff[11];
                	receiveCompleteData[10] = dataBuff[12];
                	receiveCompleteData[11] = dataBuff[13];
                	receiveCompleteData[12] = dataBuff[14];
                	receiveCompleteData[13] = dataBuff[15];
                	receiveCompleteData[14] = dataBuff[16];
                	receiveCompleteData[15] = dataBuff[17];
              	}
                
                /***********************************************/                  
                else if (receiveDtype == dType_LinkRssi)//Discovered Device
                {
                	rssi = dataBuff[2];
                	rssi = rssi - 256;
                	                	
                	#if defined(FIND_HWSERIAL1)
								  if(debugMode == 1)
								  {		
								  	DEBUG_SERIAL.print("RSSI : ");	
								  	DEBUG_SERIAL.println(rssi);									  
								  }
							    #endif  
                }
                /***********************************************/                                 
                else if (receiveDtype == dType_LinkDiscoveredDevice)//Discovered Device
                {
                  byte devIndex = dataBuff[2];
                  
				  byte devName0[20];
				  byte devName1[20];
				  byte devName2[20];
				  byte devName3[20];					
				  byte devName4[20];	
				  
                  if (devIndex == 0)
                  {
                    for (int i = 3; i <= 8; i++)
                    {
                      devAddress0[i - 3] = dataBuff[i];
                    }
                    
                    for (int i = 9; i <= 28; i++)
                    {
                    	devName0[i - 3] = dataBuff[i];
                    }
                                                            
                    devRSSI0 = dataBuff[29];
                    devFind[0] = 1; 
                  }
                  
                  
                  else if (devIndex == 1)
                  {
                    for (int i = 3; i <= 8; i++)
                    {
                      devAddress1[i - 3] = dataBuff[i];
                    }
                    
                    for (int i = 9; i <= 28; i++)
                    {
                    	devName1[i - 3] = dataBuff[i];
                    }
                    
                    devRSSI1 = dataBuff[29];
                    devFind[1] = 1;
                  }
                  
                  
                  
                  else if (devIndex == 2)
                  {
                    for (int i = 3; i <= 8; i++)
                    {
                      devAddress2[i - 3] = dataBuff[i];
                    }
                    
                    for (int i = 9; i <= 28; i++)
                    {
                    	devName2[i - 3] = dataBuff[i];
                    }
                    
                    devRSSI2 = dataBuff[29];
                    devFind[2] = 1;   
                  }
                                  
                  
                  else if (devIndex == 3)
                  {
                    for (int i = 3; i <= 8; i++)
                    {
                      devAddress3[i - 3] = dataBuff[i];
                    }
                    
                    for (int i = 9; i <= 28; i++)
                    {
                    	devName3[i - 3] = dataBuff[i];
                    }
                    
                    devRSSI3 = dataBuff[29];
                    devFind[3] = 1;   
                  }
                  
                  
                  else if (devIndex == 4)
                  {
                    for (int i = 3; i <= 8; i++)
                    {
                      devAddress4[i - 3] = dataBuff[i];
                    }
                    
                    for (int i = 9; i <= 28; i++)
                    {
                    	devName4[i - 3] = dataBuff[i];
                    }
                    
                    devRSSI4 = dataBuff[29];
                    devFind[4] = 1;   
                  }
                  
                  devCount = devFind[0] + devFind[1] + devFind[2] + devFind[3] + devFind[4];
                  
                  
                  #if defined(FIND_HWSERIAL1)
                  
					 if(debugMode == 1)
					 {								  	
                		DisplayAddress(devCount, devName0, devName1, devName2, devName3, devName4); //Address display								    
					  }
				  #endif  
							      
                }
              }              
              /***********************************************/      

              checkHeader = 0;
              cmdIndex = 0;
              ReceiveEventCheck(receiveCompleteData);
            }
          }
        }
        else
        {
          checkHeader = 0;
          cmdIndex = 0;
        }
      }
    }
  }
}
/***************************************************************************/

void CoDroneClass::PrintDroneAddress()
{	
  Send_LinkModeBroadcast(LinkBroadcast_Mute);    
  delay(100);
  
  DRONE_SERIAL.println("");
  DRONE_SERIAL.println("Connected Drone Address");
	
	for(char i = 0; i <= 4; i++)
	{
		DRONE_SERIAL.print("0x");
		DRONE_SERIAL.print(devAddressConnected[i],HEX);
		DRONE_SERIAL.print(", ");
	}
	DRONE_SERIAL.print("0x");
	DRONE_SERIAL.print(devAddressConnected[5],HEX);
}

/**********************************************************/

void CoDroneClass::DisplayAddress(byte count, byte _devName0[], byte _devName1[],byte _devName2[], byte _devName3[], byte _devName4[])
{	
  #if defined(FIND_HWSERIAL1)
  if(debugMode == 1)
  {				
  														  	
	  if (count == 1)    	DEBUG_SERIAL.print("0 : ");
	  else if (count == 2)  DEBUG_SERIAL.print("1 : ");
	  else if (count == 3)  DEBUG_SERIAL.print("2 : ");
	  else if (count == 4)  DEBUG_SERIAL.print("3 : ");
	  else if (count == 5)  DEBUG_SERIAL.print("4 : "); 
	
	  for (int i = 0; i <= 5; i++)
	  {
	    if (count == 1)
	    {
	      DEBUG_SERIAL.print(devAddress0[i], HEX);
	    	if(i < 5)	DEBUG_SERIAL.print(", ");
	    }	    
	    else if (count == 2)
	    {
	      DEBUG_SERIAL.print(devAddress1[i], HEX); 
	      if(i < 5)	DEBUG_SERIAL.print(", ");
	    }	    	
	    else if (count == 3)
	    {
	      DEBUG_SERIAL.print(devAddress2[i], HEX); 
	      if(i < 5)	DEBUG_SERIAL.print(", ");
	    }
	    
	    else if (count == 4)
	    {
	      DEBUG_SERIAL.print(devAddress3[i], HEX); 
	      if(i < 5)	DEBUG_SERIAL.print(", ");
	    }
	    else if (count == 5)
	    {
	      DEBUG_SERIAL.print(devAddress4[i], HEX); 
	      if(i < 5)	DEBUG_SERIAL.print(", ");
	    }
	  }
	   DEBUG_SERIAL.print("\t");
	  /*
	  DEBUG_SERIAL.print("\t");
	  DEBUG_SERIAL.print("NAME :");
	   
 		for (int i = 0; i <= 19; i++)
	  {
	    if (count == 1)
	    {
	      DEBUG_SERIAL.write(_devName0[i]);
	    }	    
	    else if (count == 2)
	    {
	      DEBUG_SERIAL.write(_devName1[i]);
	    }	    	
	    else if (count == 3)
	    {
	      DEBUG_SERIAL.write(_devName2[i]);
	    }
	    
	    else if (count == 4)
	    {
	      DEBUG_SERIAL.write(_devName3[i]);
	    }	    
	    
	    else if (count == 5)
	    {
	      DEBUG_SERIAL.write(_devName4[i]);
	    }	    
	  }	 	  	  
	  	  	  
	  DEBUG_SERIAL.print(" RSSI : ");
	  */
  	if (count == 1)     	DEBUG_SERIAL.println(devRSSI0 - 256);
  	else if (count == 2)	DEBUG_SERIAL.println(devRSSI1 - 256);
  	else if (count == 3)	DEBUG_SERIAL.println(devRSSI2 - 256);	  	 
  	else if (count == 4)	DEBUG_SERIAL.println(devRSSI3 - 256);
  	else if (count == 5)	DEBUG_SERIAL.println(devRSSI4 - 256);	  	 	 	
    }	
	#endif    

}

/**********************************************************/


void CoDroneClass::LED_Start()
{
	if(displayMode == 1)
	{
		LED_Move_Slide();			
		LED_Standard();
	}
}

void CoDroneClass::LED_PORTC(int value)
{
	if(displayMode == 1)
	{
		PORTC |= value;
	}
}

void CoDroneClass::LED_DDRC(int value)
{
	if(displayMode == 1)
	{
		DDRC |= value;
	}
}

void CoDroneClass::LED_Standard()
{
	if(displayMode == 1)
	{
		DDRC = 	0b01100110;
		PORTC = 0x00;
	}
}

void CoDroneClass::LED_Move_Radar(byte display)
{
	if(displayMode == 1)
	{
		PORTC = (0b1<<display) | (0b10000000>>display);	 
	}
}

void CoDroneClass::LED_Move_Slide()
{
	if(displayMode == 1)
	{
	  int led_sign = 0;
	  DDRC =  0xff;
	  while (led_sign < 8)
	  {
	    PORTC = ((1 << (led_sign++)) - 1);
	    delay(50);
	  }
	  while (led_sign > -2)
	  {
	    PORTC = ((1 << (led_sign--)) - 1);
	    delay(50);
	  }
	}
}

void CoDroneClass::LED_Connect()
{	
	if(displayMode == 1)
	{
		DDRC = 0b00100100;
		PORTC = 0b00100100;	
	}
}

void CoDroneClass::LED_Blink(int time, int count)
{
	if(displayMode == 1)
	{
    for (int i = 0; i < count; i++)
    {
      digitalWrite(13, HIGH);
      digitalWrite(16, HIGH);
      
      delay(time);      

      digitalWrite(13, LOW);      
      digitalWrite(16, LOW);
      
      delay(time);
    }
  }
}

/**********************************************************/

void CoDroneClass::DisplayRSSI()
{	
	if(displayMode == 1)
	{
		Send_RSSI_Polling(PollingStart);
		
		delay(300);
		
		DDRC = 0xff;
		PORTC = 0x00;
	   
		while(1)
		{
			Receive();
			
			int _rssi = RSSI * -1;
			_rssi = _rssi / 10;
			_rssi = _rssi - 2;
			
			if (_rssi < 0 )_rssi  = 0;
			if (_rssi > 7) _rssi = 7;
			
			PORTC = 0b1;
			
			for (int i = 0; i < _rssi ; i++)
			{
				PORTC |= PORTC << i;
			}
		}
	}  
}


/**********************************************************/
int CoDroneClass::LowBatteryCheck(byte value)
{	
	int bat = -1;
	timeOutRetry = 0;
	CoDrone.Request_DroneState();
	
	PreviousMillis = millis();
		  	
	while(1)
	{		
		Receive();
		
		if (CoDrone.droneState[0] != 0 )
		{	
			battery = droneState[6];
			bat = droneState[6];
			
			if(bat < value)
			{
			BeepWarning(5);
			}						
			droneState[0] = 0;
			droneState[1] = 0;
			droneState[2] = 0;
			droneState[3] = 0;
			droneState[4] = 0;
			droneState[5] = 0;
			droneState[6] = 0;		  
			break;
		}
	  
		else if (TimeCheck(1000))	//time out
		{
			timeOutRetry ++;
			if(timeOutRetry <3)
			{
				bat = -1;
				droneState[0] = 0;
				droneState[1] = 0;
				droneState[2] = 0;
				droneState[3] = 0;
				droneState[4] = 0;
				droneState[5] = 0;
				droneState[6] = 0;
				break;	 
			}
			else
			{
				CoDrone.Request_DroneState();
				delay(50);	
				PreviousMillis = millis();				
			}
		}		  
	}
	delay(50);
	return bat;
}

/**********************************************************/

void CoDroneClass::LinkStateCheck()	//ready or connected ?
{
	linkState = 0;
	Send_LinkState();
	
	delay(50);
	
	while (linkState <= 0) 	
	{
		Receive();
	}  
}

/**********************************************************/

void CoDroneClass::ReceiveEventCheck(byte _completeData[])
{
	/***************************************************************/
		
	if(receiveComplete > 0)
	{
	/**************************************************************/	
		
		if (receiveDtype == dType_State)
		{
			if (_completeData[0] != 0 )
			{
					 		  				
	        droneState[0] = _completeData[0];
          	droneState[1] = _completeData[1];
          	droneState[2] = _completeData[2];
          	droneState[3] = _completeData[3];
          	droneState[4] = _completeData[4];
          	droneState[5] = _completeData[5];	
          	droneState[6] = _completeData[6];	  				
				
				  #if defined(FIND_HWSERIAL1)				  
				  if(debugMode == 1)
				  { 
						DEBUG_SERIAL.println("");
						DEBUG_SERIAL.println("- Request Drone State");
								
						DEBUG_SERIAL.print("ModeDrone \t");
						
						if(droneState[0] == dMode_None)								DEBUG_SERIAL.println("None");
						else if(droneState[0] == dMode_Flight)						DEBUG_SERIAL.println("Flight");
						else if(droneState[0] == dMode_FlightNoGuard)				DEBUG_SERIAL.println("FlightNoGuard");
						else if(droneState[0] == dMode_FlightFPV)					DEBUG_SERIAL.println("FlightFPV");
						else if(droneState[0] == dMode_Drive)						DEBUG_SERIAL.println("Drive");
						else if(droneState[0] == dMode_DriveFPV)					DEBUG_SERIAL.println("DriveFPV");
						else if(droneState[0] == dMode_Test)						DEBUG_SERIAL.println("Test");
						
						DEBUG_SERIAL.print("ModeVehicle \t");	
							
						if(droneState[1] == vMode_None)								DEBUG_SERIAL.println("None");
						else if(droneState[1] == vMode_Boot)						DEBUG_SERIAL.println("Boot");
						else if(droneState[1] == vMode_Wait)						DEBUG_SERIAL.println("Wait");
						else if(droneState[1] == vMode_Ready)						DEBUG_SERIAL.println("Ready");
						else if(droneState[1] == vMode_Running)						DEBUG_SERIAL.println("Running");
						else if(droneState[1] == vMode_Update)						DEBUG_SERIAL.println("Update");
						else if(droneState[1] == vMode_UpdateComplete)				DEBUG_SERIAL.println("UpdateComplete");
						else if(droneState[1] == vMode_Error)						DEBUG_SERIAL.println("Error");
						
						DEBUG_SERIAL.print("ModeFlight \t");	
						
						if(droneState[2] == fMode_None)								DEBUG_SERIAL.println("None");
						else if(droneState[2] == fMode_Ready)						DEBUG_SERIAL.println("Ready");
						else if(droneState[2] == fMode_TakeOff)						DEBUG_SERIAL.println("TakeOff");
						else if(droneState[2] == fMode_Flight)						DEBUG_SERIAL.println("Flight");
						else if(droneState[2] == fMode_Flip)						DEBUG_SERIAL.println("Flip");
						else if(droneState[2] == fMode_Stop)						DEBUG_SERIAL.println("Stop");
						else if(droneState[2] == fMode_Landing)						DEBUG_SERIAL.println("Landing");
						else if(droneState[2] == fMode_Reverse)						DEBUG_SERIAL.println("Reverse");
						else if(droneState[2] == fMode_Accident)					DEBUG_SERIAL.println("Accident");
						else if(droneState[2] == fMode_Error)						DEBUG_SERIAL.println("Error");
								
						DEBUG_SERIAL.print("ModeDrive \t");	
										
						if(droneState[3] == dvMode_None)							DEBUG_SERIAL.println("None");
						else if(droneState[3] == dvMode_Ready)						DEBUG_SERIAL.println("Ready");
						else if(droneState[3] == dvMode_Start)						DEBUG_SERIAL.println("Start");
						else if(droneState[3] == dvMode_Drive)						DEBUG_SERIAL.println("Drive");
						else if(droneState[3] == dvMode_Stop)						DEBUG_SERIAL.println("Stop");
						else if(droneState[3] == dvMode_Accident)					DEBUG_SERIAL.println("Accident");
						else if(droneState[3] == dvMode_Error)						DEBUG_SERIAL.println("Error");
											
						DEBUG_SERIAL.print("SensorOrientation \t");			
						
						if(droneState[4] == senOri_None)							DEBUG_SERIAL.println("None");
						else if(droneState[4] == senOri_Normal)						DEBUG_SERIAL.println("Normal");
						else if(droneState[4] == senOri_ReverseStart)				DEBUG_SERIAL.println("ReverseStart");
						else if(droneState[4] == senOri_Reverse)					DEBUG_SERIAL.println("Reverse");
									
						DEBUG_SERIAL.print("Coordinate \t");	
																	
						if(droneState[5] == cSet_None)								DEBUG_SERIAL.println("None");
						else if(droneState[5] == cSet_Absolute)						DEBUG_SERIAL.println("Absolute");
						else if(droneState[5] == cSet_Relative)						DEBUG_SERIAL.println("Relative");
						
						DEBUG_SERIAL.print("Battery \t");	
						DEBUG_SERIAL.println(droneState[6]);
										
			  }	
		    #endif		
		    				
			}		
		}
		
		/**************************************************************/
		
	 else if (receiveDtype == dType_IrMessage)		//IrMessage
     {
     	
   	  byte irMassageDirection	=	_completeData[0];
   	  
   	  unsigned long _irMassge[4];
   	  
   	  _irMassge[0] = _completeData[1];
   	  _irMassge[1] = _completeData[2];
   	  _irMassge[2] = _completeData[3];
   	  _irMassge[3] = _completeData[4];
   	     	  
   	  irMassageReceive	= ((_irMassge[3] << 24) | (_irMassge[2] << 16) | (_irMassge[1] << 8) | (_irMassge[0]  & 0xff));
   	  
  	  #if defined(FIND_HWSERIAL1)		
  	  
		  if(debugMode == 1)
		  { 			  			  	
				DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- IrMassage");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[1],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[2],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[3],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[4],HEX);		
				DEBUG_SERIAL.println(" ]");
				
				DEBUG_SERIAL.print("IrMassageDirection\t");					
				DEBUG_SERIAL.print(irMassageDirection);
				
				if(irMassageDirection == 1)			DEBUG_SERIAL.println(" (Left)");
				else if (irMassageDirection == 2)	DEBUG_SERIAL.println(" (Front)");
				else if (irMassageDirection == 3)	DEBUG_SERIAL.println(" (Right)");
				else if (irMassageDirection == 4)	DEBUG_SERIAL.println(" (Rear)");
				else DEBUG_SERIAL.println("None");
								
				DEBUG_SERIAL.print("IrMassageReceive\t");	
				DEBUG_SERIAL.println(irMassageReceive);
																					
			}	
		  #endif		
     }                          
			/**************************************************************/
	  else if (receiveDtype == dType_Attitude)
	  {
				attitudeRoll	= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
				attitudePitch	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
				attitudeYaw		= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
				
				receiveAttitudeSuccess = 1;
				
	  	  #if defined(FIND_HWSERIAL1)				  
	  	    	  														  	  		
			  if(debugMode == 1)
			  {
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- Attitude");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);				
					DEBUG_SERIAL.println(" ]");
					
					DEBUG_SERIAL.print("ROLL\t");	
					DEBUG_SERIAL.println(attitudeRoll);
					
					DEBUG_SERIAL.print("PITCH\t");	
					DEBUG_SERIAL.println(attitudePitch);
					
					DEBUG_SERIAL.print("YAW\t");	
					DEBUG_SERIAL.println(attitudeYaw);
																	
				}	
			  #endif				  
	  }	 
	  
		/**************************************************************/	
		else if (receiveDtype == dType_GyroBias)
	    {  				  	
	  	  #if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  	
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- GyroBias");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);				
					DEBUG_SERIAL.println(" ]");
					
					int GyroBias_Roll		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
					int GyroBias_Pitch		= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
					int GyroBias_Yaw		= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
					
					DEBUG_SERIAL.print("GyroBias ROLL\t");	
					DEBUG_SERIAL.println(GyroBias_Roll);
					
					DEBUG_SERIAL.print("GyroBias PITCH\t");	
					DEBUG_SERIAL.println(GyroBias_Pitch);
					
					DEBUG_SERIAL.print("GyroBias YAW\t");	
					DEBUG_SERIAL.println(GyroBias_Yaw);
																	
				}	
			  #endif				  
	  }	
	  
	/**************************************************************/	
	  
		else if (receiveDtype == dType_TrimAll)
		{				
			TrimAll_Roll			= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			TrimAll_Pitch			= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			TrimAll_Yaw				= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			TrimAll_Throttle		= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
			TrimAll_Wheel			= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
			
			 #if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  	
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- TrimAll");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[6],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[7],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[8],HEX);			
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[9],HEX);				
					DEBUG_SERIAL.println(" ]");			
						  															
					DEBUG_SERIAL.print("Trim ROLL\t");	
					DEBUG_SERIAL.println(TrimAll_Roll);
					
					DEBUG_SERIAL.print("Trim PITCH\t");	
					DEBUG_SERIAL.println(TrimAll_Pitch);
					
					DEBUG_SERIAL.print("Trim YAW\t");	
					DEBUG_SERIAL.println(TrimAll_Yaw);
			  
					DEBUG_SERIAL.print("Trim Throttle\t");	
					DEBUG_SERIAL.println(TrimAll_Throttle);
			  
					DEBUG_SERIAL.print("Trim Wheel\t");	
					DEBUG_SERIAL.println(TrimAll_Wheel);
			  }	
			 #endif				  
		}
	/**************************************************************/	
	  else if (receiveDtype == dType_TrimFlight)		//
    { 
    		TrimAll_Roll		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			TrimAll_Pitch		= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			TrimAll_Yaw			= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			TrimAll_Throttle	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
								
    	
	 		#if defined(FIND_HWSERIAL1)	
	 			  
			if(debugMode == 1)
			{ 		
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- TrimFlight");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[6],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[7],HEX);				
					DEBUG_SERIAL.println(" ]");			
    	    						
					DEBUG_SERIAL.print("Trim ROLL\t");	
					DEBUG_SERIAL.println(TrimAll_Roll);
					
					DEBUG_SERIAL.print("Trim PITCH\t");	
					DEBUG_SERIAL.println(TrimAll_Pitch);
					
					DEBUG_SERIAL.print("Trim YAW\t");	
					DEBUG_SERIAL.println(TrimAll_Yaw);
			  
					DEBUG_SERIAL.print("Trim Throttle\t");	
					DEBUG_SERIAL.println(TrimAll_Throttle);
			  
			 }	
			 #endif				  
  	}    
  	        	
		/**************************************************************/	      	
	  else if (receiveDtype == dType_TrimDrive)		//
    {
    	
    	 #if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 	
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- TrimDrive");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.println(" ]");
					
			  		 TrimAll_Wheel			= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			  	
					DEBUG_SERIAL.print("Trim Wheel\t");	
					DEBUG_SERIAL.println(TrimAll_Wheel);
				}	
			  #endif			  	
		}
	  
		/**************************************************************/	
		else if(receiveDtype == dType_ImuRawAndAngle)
		{
			 #if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  	
					DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- ImuRawAndAngle");
					DEBUG_SERIAL.print("[ ");
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[6],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[7],HEX);				
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[8],HEX);				
					DEBUG_SERIAL.println(" ]");
										
					int ImuAccX	= _completeData[0];
					int ImuAccY	= _completeData[1];
					int ImuAccZ	= _completeData[2];			
							
					int ImuGyroRoll		= _completeData[3];
					int ImuGyroPitch	= _completeData[4];
					int ImuGyroYaw		= _completeData[5];	
									
					int ImuAngleRoll	= _completeData[6];
					int ImuAnglePitch	= _completeData[7];
					int ImuAngleYaw		= _completeData[8];
										
					DEBUG_SERIAL.print("AccX\t");	
					DEBUG_SERIAL.println(ImuAccX);
					
					DEBUG_SERIAL.print("AccY\t");	
					DEBUG_SERIAL.println(ImuAccY);
					
					DEBUG_SERIAL.print("AccZ\t");	
					DEBUG_SERIAL.println(ImuAccZ);
					
					
					DEBUG_SERIAL.print("GyroRoll\t");	
					DEBUG_SERIAL.println(ImuGyroRoll);
					
					DEBUG_SERIAL.print("GyroPitch\t");	
					DEBUG_SERIAL.println(ImuGyroPitch);
					
					DEBUG_SERIAL.print("GyroYaw \t");	
					DEBUG_SERIAL.println(ImuGyroYaw);
										
					DEBUG_SERIAL.print("AngleRoll\t");	
					DEBUG_SERIAL.println(ImuAngleRoll);
					
					DEBUG_SERIAL.print("AnglePitch\t");	
					DEBUG_SERIAL.println(ImuAnglePitch);
					
					DEBUG_SERIAL.print("AngleYaw\t");	
					DEBUG_SERIAL.println(ImuAngleYaw);
												
				}	
			  #endif	
		}
		
	 /**************************************************************/		
		
		else if(receiveDtype == dType_Pressure)
		{
			#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  		DEBUG_SERIAL.println("");                	                
					DEBUG_SERIAL.println("- Pressure");
					DEBUG_SERIAL.print("[ ");
					
					DEBUG_SERIAL.print(_completeData[0],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[1],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[2],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[3],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[4],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[5],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[6],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[7],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[8],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[9],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[10],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[11],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[12],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[13],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[14],HEX);
					DEBUG_SERIAL.print(", ");
					DEBUG_SERIAL.print(_completeData[15],HEX);
					
					DEBUG_SERIAL.println(" ]");
					
			  	long pressure1	= ((_completeData[3] << 32) |(_completeData[2] << 16) |(_completeData[1] << 8) | (_completeData[0]  & 0xff));
			//  			  			
			  						  						  						  			
			  	DEBUG_SERIAL.print("pressure1\t");	
				DEBUG_SERIAL.println(pressure1);
			  	
			  }	
			  #endif	
		}
		/**************************************************************/		
		else if (receiveDtype ==  dType_ImageFlow)
 		{ 	
 			#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  	DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- ImageFlow");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[1],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[2],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[3],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[4],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[5],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[6],HEX);					
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[7],HEX);
			  	DEBUG_SERIAL.println(" ]");
			  				  				  	
			  	long SumX0	= _completeData[0];
			  	long SumX1	= _completeData[1];
			  	long SumX2	= _completeData[2];
			  	long SumX3	= _completeData[3];
			  	
			  	long SumY0	= _completeData[4];
			  	long SumY1	= _completeData[5];
			  	long SumY2	= _completeData[6];
			  	long SumY3	= _completeData[7];			  				  	
			  	
			  	long fVelocitySumX	= ((SumX3 << 32) |(SumX2 << 16) |(SumX1 << 8) | (SumX0  & 0xff));
			  	long fVelocitySumY	= ((SumY3 << 32) |(SumY2 << 16) |(SumY1 << 8) | (SumY0  & 0xff));
			  				  					  						  						  		
			  	DEBUG_SERIAL.print("fVelocitySumX\t");	
				DEBUG_SERIAL.println(fVelocitySumX);
					
				DEBUG_SERIAL.print("fVelocitySumY\t");	
				DEBUG_SERIAL.println(fVelocitySumY);
			  }	
	 		#endif	
		  
		}
		/**************************************************************/		
		else if (receiveDtype ==  dType_Button)
		{				
				#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  	DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- Button");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
			  	DEBUG_SERIAL.println(" ]");
			  }	
	 			#endif	
	 			
		}
		/**************************************************************/		
		else if (receiveDtype ==  dType_Batery)
		{
				#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  	DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- Batery");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[1],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[2],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[3],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[4],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[5],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[6],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[7],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[8],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[9],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[10],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[11],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[12],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[13],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[14],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[15],HEX);										
			  	DEBUG_SERIAL.println(" ]");
			  	
			  }	
	 			#endif	
	 			
 		
		}
		/**************************************************************/	
    else if (receiveDtype ==  dType_Motor)
    {
			#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  	DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- Motor");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[1],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[2],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[3],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[4],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[5],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[6],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[7],HEX);
				DEBUG_SERIAL.print(", ");	
				DEBUG_SERIAL.print(_completeData[8],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[9],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[10],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[11],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[12],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[13],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[14],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[15],HEX);										
			  	DEBUG_SERIAL.println(" ]");
			  				  	
			  	int m1_forward	= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			  	int m1_reverse	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			  	 	
			  	int m2_forward	= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			  	int m2_reverse	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
			  	 	
			  	int m3_forward	= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
			  	int m3_reverse	= ((_completeData[11] << 8) | (_completeData[10]  & 0xff));
			  	 		 				  	 	
			  	int m4_forward	= ((_completeData[13] << 8) | (_completeData[12]  & 0xff));
			  	int m4_reverse	= ((_completeData[15] << 8) | (_completeData[14]  & 0xff));

			  	DEBUG_SERIAL.println("[1]	[2]");	
			  	DEBUG_SERIAL.println("  [ ]	 ");	
			  	DEBUG_SERIAL.println("[4]	[3]");	
								
			  	DEBUG_SERIAL.print("m1_forward\t");	
					DEBUG_SERIAL.println(m1_forward);
					
			  	DEBUG_SERIAL.print("m1_reverse\t");	
					DEBUG_SERIAL.println(m1_reverse);
					
			  	DEBUG_SERIAL.print("m2_forward\t");	
					DEBUG_SERIAL.println(m2_forward);
						 	 	
			  	DEBUG_SERIAL.print("m2_reverse\t");	
					DEBUG_SERIAL.println(m2_reverse);
					
			  	DEBUG_SERIAL.print("m3_forward\t");	
					DEBUG_SERIAL.println(m3_forward);
					
			  	DEBUG_SERIAL.print("m3_reverse\t");	
					DEBUG_SERIAL.println(m3_reverse);
					
			  	DEBUG_SERIAL.print("m4_forward\t");	
					DEBUG_SERIAL.println(m4_forward);
						 	 	
			  	DEBUG_SERIAL.print("m4_reverse\t");	
					DEBUG_SERIAL.println(m4_reverse);
														 	 	 	 	 	
			  }	
		 	#endif	
		 			
    }
		/**************************************************************/	
		else if (receiveDtype == dType_Temperature)
    {
			#if defined(FIND_HWSERIAL1)				  
			  if(debugMode == 1)
			  { 			  		
			  	DEBUG_SERIAL.println("");                	                
				DEBUG_SERIAL.println("- Temperature");
				DEBUG_SERIAL.print("[ ");
				DEBUG_SERIAL.print(_completeData[0],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[1],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[2],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[3],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[4],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[5],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[6],HEX);
				DEBUG_SERIAL.print(", ");
				DEBUG_SERIAL.print(_completeData[7],HEX);										
			  	DEBUG_SERIAL.println(" ]");
			  }	
			  
		 	#endif	
		 			
    }
		
	 /**************************************************************/		
								
		else if (receiveDtype == dType_LinkState)
		{				
				#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 
					DEBUG_SERIAL.print(receiveLinkState);			
				}
				#endif
				
				if(receiveLinkState == linkMode_None)	
				{
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - None");	
					}
					#endif
				}
				else if(receiveLinkState == linkMode_Boot)	
				{
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - Boot");	
					}
					#endif					
				}
				else if(receiveLinkState == linkMode_Ready)	
				{	
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - Ready");	
					}
					#endif
				}
				else if(receiveLinkState == linkMode_Connecting)
				{					
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - Connecting");	
					}
					#endif
				}
				else if(receiveLinkState == linkMode_Connected)
				{
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - Connected");
					}
					#endif					
				}	
				else if(receiveLinkState == linkMode_Disconnecting)
				{
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - Disconnecting");	
					}
					#endif
				}
				else if(receiveLinkState == linkMode_ReadyToReset)	
				{
					#if defined(FIND_HWSERIAL1)				  
					if(debugMode == 1)
					{ 
						DEBUG_SERIAL.println(" : linkMode - ReadyToReset");		
					}
					#endif					
				}

				linkState = receiveLinkState;			  		  		
		}		
	/**************************************************************/						
				
	  else if ((receiveDtype == dType_LinkEvent) && (receiveEventState > 0))
	  {  	
	  	
	  	#if defined(FIND_HWSERIAL1)				  
			if(debugMode == 1)
			{ 			  		
	 		 	DEBUG_SERIAL.print(receiveEventState);
		  	}	
		#endif
		 	
		if (receiveEventState == linkEvent_None)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 			  		
		       		DEBUG_SERIAL.println(" : linkEvent - None");
			  	}	
		 	#endif	    
	    }  
	      
		else if (receiveEventState == linkEvent_SystemReset)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 			  		
		       		DEBUG_SERIAL.println(" : linkEvent - SystemReset");
			  	}	
		 	#endif	
	    }
	    
		else if (receiveEventState == linkEvent_Initialized)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 			  		
		       		DEBUG_SERIAL.println(" : linkEvent - Initialized");
			  	}	
		 	#endif		    	
	    }
	    
		else if (receiveEventState == linkEvent_Scanning)
	    {
	    	if(discoverFlag == 1) discoverFlag = 2;
	    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 		
		      		 DEBUG_SERIAL.println(" : linkEvent - Scanning");
			  	}	
		 	#endif		    
	    }
	    
		else if (receiveEventState == linkEvent_ScanStop)
	    {
	    	
	    	if(discoverFlag == 2)
	    	{    		
	    		
	    	//	if(devCount > 0)
	    	//	{
	    			discoverFlag = 3;
	    	//	}
	    	//	else
	    	//	{
	    	//		discoverFlag = 4;
	    	//	}    		 
	    	} 
	    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 		
		       		DEBUG_SERIAL.println(" : linkEvent - ScanStop");
			  	}	
		 	#endif	
	    }
	    	    
		else if (receiveEventState == linkEvent_FoundDroneService)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{ 			  	
		       		DEBUG_SERIAL.println(" : linkEvent - FoundDroneService");
			  	}	
		 	#endif		  
	    }
	    
		else if (receiveEventState == linkEvent_Connecting)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
		       		DEBUG_SERIAL.println(" : linkEvent - Connecting");
			  	}	
		 	#endif
	    }
		
		else if (receiveEventState == linkEvent_Connected)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - Connected");
		  		}	
		 	#endif		 		
	    }
	    
		else if (receiveEventState == linkEvent_ConnectionFaild)
	    {	    		    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
		       		DEBUG_SERIAL.println(" : linkEvent - ConnectionFaild");
			  	}	
		 	#endif	
	    }
	    
		else if (receiveEventState == linkEvent_ConnectionFaildNoDevices)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - ConnectionFaildNoDevices");
		  		}	
		 	#endif
	    }
	    
		else if (receiveEventState == linkEvent_ConnectionFaildNotReady)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - ConnectionFaildNotReady");
		  		}	
		 		#endif
	    }
	    	    
	    else if (receiveEventState == linkEvent_PairingStart)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - PairingStart");
		  		}	
		 	#endif		 		
	    }
	    
	    else if (receiveEventState == linkEvent_PairingSuccess)
	    {	    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - PairingSuccess");
		  		}	
		 	#endif				 		
	    }
	    else if (receiveEventState == linkEvent_PairingFaild)
	    {	    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - PairingFaild");
		  		}	
		 	#endif			 		
	    }	    
	    
	    else if (receiveEventState == linkEvent_BondingSuccess)
	    {	    	
				#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - BondingSuccess");
		  		}	
		 		#endif		 	
	    }	    
	    
	    else if (receiveEventState == linkEvent_LookupAttribute)
	    {	    	
				#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - LookupAttribute");
		  		}	
		 		#endif	
	    }	    
	    
	    else if (receiveEventState == linkEvent_RssiPollingStart)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
			       DEBUG_SERIAL.println(" : linkEvent - RssiPollingStart");
				}	
		 		#endif	
	    }  
	      
	    else if (receiveEventState == linkEvent_RssiPollingStop)
	    {	    	
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	      			DEBUG_SERIAL.println(" : linkEvent - RssiPollingStop");
		  		}	
		 		#endif	
	    }	    
	    
	    else if (receiveEventState == linkEvent_DiscoverService)
	    {	    	
				#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverService");
		  		}	
		 		#endif	
	    }
	    
	    else if (receiveEventState == linkEvent_DiscoverCharacteristic)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverCharacteristic");
		  		}	
		 		#endif	
	    }
	    else if (receiveEventState == linkEvent_DiscoverCharacteristicDroneData)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverCharacteristicDroneData");
		  		}	
		 		#endif	
	    }    
	    else if (receiveEventState == linkEvent_DiscoverCharacteristicDroneConfig)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverCharacteristicDroneConfig");
		  		}	
		 		#endif	
	    }
	    else if (receiveEventState == linkEvent_DiscoverCharacteristicUnknown)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverCharacteristicUnknown");
		  		}	
		 		#endif	
	    }
	    else if (receiveEventState == linkEvent_DiscoverCCCD)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
	       			DEBUG_SERIAL.println(" : linkEvent - DiscoverCCCD");
		  		}	
		 		#endif		    
	    }
	          
	    else if (receiveEventState == linkEvent_ReadyToControl)
	    {	    	
	    	
			  #if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
		     		DEBUG_SERIAL.println(" : linkEvent - ReadyToControl");
		  		}	
		 		#endif	    	
	    	
	      if(connectFlag == 1)
	      {
				connectFlag = 0;         
									
				EEPROM.write(EEP_AddressCheck, 0x01);						
				for (int i = 0; i <= 5; i++)
				{
				//	devAddressConnected[i] = devAddressBuf[i];
					EEPROM.write(EEP_AddressFirst + i, devAddressBuf[i]);				
				}
			}			  
			LED_Connect();
			pairing = true;    
			delay(500);
	    }
	        
	    else if (receiveEventState == linkEvent_Disconnecting)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
		       		DEBUG_SERIAL.println(" : linkEvent - Disconnecting");
			  	}	
		 		#endif	  
	    }
	    else if (receiveEventState == linkEvent_Disconnected)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
		       		DEBUG_SERIAL.println(" : linkEvent - Disconnected");
			  	}	
		 		#endif	 
	    }
	    
	    
	    else if (receiveEventState == linkEvent_GapLinkParamUpdate)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - GapLinkParamUpdate");
				}	
		 		#endif	 		 		
	    }
	 
	    else if (receiveEventState == linkEvent_RspReadError)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - RspReadError");
				}	
		 		#endif	
	    }
	    
	    else if (receiveEventState == linkEvent_RspReadSuccess)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - RspReadSuccess");
				}	
		 		#endif			 		
	    }
	    
	    else if (receiveEventState == linkEvent_RspWriteError)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - RspWriteError");
				}	
		 		#endif	
	    }
	    
	    else if (receiveEventState == linkEvent_RspWriteSuccess)
	    {
	    	if(sendCheckFlag == 1)
	    	{
	    		sendCheckFlag = 2;
	  		}
	  		
	    	#if defined(FIND_HWSERIAL1)			
	    	 // 	Serial.println(millis());	  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - RspWriteSuccess");
				}	
		 		#endif	
	    }
	         
	    else if (receiveEventState == linkEvent_SetNotify)
	    {
	    	#if defined(FIND_HWSERIAL1)				  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - SetNotify");
				}	
		 		#endif	
	    }
	    
	    else if (receiveEventState == linkEvent_Write)
	    {	
	    	if(sendCheckFlag == 2)
	    	{
	    		sendCheckFlag = 3;
	 			}
	 			
	    	#if defined(FIND_HWSERIAL1)		
	    	  // 	Serial.println(millis());		  
				if(debugMode == 1)
				{
					DEBUG_SERIAL.println(" : linkEvent - Write");
				}	
		 	#endif	
	    }  
	 
	  }	  
  	  
	receiveEventState = -1;	  
	receiveComplete = -1;
	// receiveLength = -1;
	receiveLinkState = -1;
	//  receiveDtype = -1;	
  	  
	}
}
/*********************************************************/

void CoDroneClass::ButtonPreesHoldWait(int button)
{
  do {
    delay(10);
  }    while (digitalRead(button));
}


void CoDroneClass::ButtonPreesHoldWait(int button1, int button2)
{
  do {
    delay(10);
  }    while (digitalRead(button1) && digitalRead(button2));
}

/*********************************************************/

boolean CoDroneClass::TimeCheck(word interval) //milliseconds
{
  boolean time = false;
  unsigned long currentMillis = millis();
  if (currentMillis - PreviousMillis > interval)
  {
    PreviousMillis = currentMillis;
    time = true;
  }
  return time;
}


boolean CoDroneClass::TimeOutSendCheck(word interval) //milliseconds
{
  boolean time = false;
  unsigned long currentMillis = millis();
  if (currentMillis - timeOutSendPreviousMillis > interval)
  {
    timeOutSendPreviousMillis = currentMillis;
    time = true;
  }
  return time;
}

/*********************************************************/

int CoDroneClass::AnalogScaleChange(int analogValue)
{	
    int ScaleChange = map(analogValue, 0, 1023, -100, 100);
    if ((ScaleChange > -1 * analogOffset) && (ScaleChange < analogOffset)) ScaleChange = 0;        
    return ScaleChange;
}

/*********************************************************/

void CoDroneClass::BeepWarning(int count) 
{
	for (int i = 0; i < count; i++)
	{
		CoDrone.Buzz(2000, 4);
		delay(400);
	}
}

/*********************************************************/
void CoDroneClass::Buzz(long frequency, int tempo) 
{
 	DDRD |= 0b10000000;
 		
	int length = 1000/tempo;  					// TEMPO
	long delayValue = 1000000/frequency/2; 		// calculate the delay value between transitions
	long numCycles = frequency * length/ 1000; 	// calculate the number of cycles for proper timing
	
	for (long i=0; i < numCycles; i++)
	{ 		
		PORTD |= 0b10000000;		
		while(!TimeCheckBuzz(delayValue));		
		PORTD &=~0b10000000;					
		while(!TimeCheckBuzz(delayValue));
	}
}

boolean CoDroneClass::TimeCheckBuzz(word interval) //micros seconds
{
  boolean time = false; 
  unsigned long currentTimes = micros();
  if(currentTimes - PreviousBuzz >= interval) 
  {
    PreviousBuzz = currentTimes;
    time = true;
  }
  return time;
}

/***************************************************************************/
int CoDroneClass::GestureCheckPosition(int sen1, boolean act1, int sen2, boolean act2)
{
	boolean state = false;
	int val1,val2;
	boolean ck1, ck2;
	
	val1 = analogRead(sen1);
	val2 = analogRead(sen2);	
	
	//---------------------------------------------------------------------------------------//         	
	if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);		// ▲UP
	else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ▼down
	if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);		// ▲UP	
	else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ▼down	
	//---------------------------------------------------------------------------------------//
	
	if(ck1 && ck2)	state = true;	
	
	return state;
}

int CoDroneClass::GestureReturnPositon(int sen1, int sen2)
{
	int val1,val2;
	boolean ck1, ck2;
	
	val1 = analogRead(sen1);
	val2 = analogRead(sen2);
	
	ck1 = ((val1 > JOY_UP_RETURN_LIMIT) || (val1 < JOY_DOWN_RETURN_LIMIT));		// MIDDLE
	ck2 = ((val2 > JOY_UP_RETURN_LIMIT) || (val2 < JOY_DOWN_RETURN_LIMIT));		// MIDDLE
	
	return ck1 || ck2;
}

/***************************************************************************/

//- ex // if (CoDrone.gestureFuntion(A4, LOW, A6, LOW, &CoDroneClass::FlightEvent, Stop)); 
int CoDroneClass::GestureFuntion(int sen1, boolean act1, int sen2, boolean act2,void(CoDroneClass::*pf)(byte),byte command)
{	
	boolean state = false;
	int val1,val2;
	boolean ck1, ck2;
	
	val1 = analogRead(sen1);
	val2 = analogRead(sen2);	
	
	//---------------------------------------------------------//         	
	if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);			// ▲UP
	else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ▼down
	if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);			// ▲UP	
	else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ▼down	
	//---------------------------------------------------------//
	
	if(ck1 && ck2)
	{
		while (ck1 && ck2)
		{				
			(CoDrone.*pf)(command);
			delay(50);
			
			val1 = analogRead(sen1);
			val2 = analogRead(sen2);	 
			         	
			if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);			// ▲UP
			else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ▼down
			if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);			// ▲UP	
			else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ▼down						
		}	
		
		while (ck1 || ck2)	//return
	  {
			val1 = analogRead(sen1);
			val2 = analogRead(sen2);	 
			         	
			if(act1 == HIGH)	ck1 = (val1 > JOY_UP_RETURN_LIMIT);		// ▲UP
			else				ck1 = (val1 < JOY_DOWN_RETURN_LIMIT);	// ▼down
			if(act2 == HIGH)	ck2 = (val2 > JOY_UP_RETURN_LIMIT);		// ▲UP	
			else				ck2 = (val2 < JOY_DOWN_RETURN_LIMIT);	// ▼down	
	  }
	  state = true;
	}
	return state;
}



void CoDroneClass::SequenceDelay(int setTime)
{
	long baseTime = millis();			
	int _interval = 120; 		//send term

	long _previousMillis;	
	unsigned long _currentMillis = millis();

	delay(_interval);

	while ((setTime - _interval) > (millis() - baseTime))
	{
		_currentMillis = millis();		
		if (_currentMillis - _previousMillis > _interval)
		{
		    _previousMillis = _currentMillis;
		 	
		 	roll 	=	prevControl[0];
			pitch 	=	prevControl[1];
			yaw 	=	prevControl[2];
			throttle =	prevControl[3];
			
		  	Control();
		  	_previousMillis = millis();	 
		}
	}

	delay(_interval);
	CoDrone.Control();
	delay(_interval);
}


/*********************************************************/

CoDroneClass CoDrone;                                                                                             