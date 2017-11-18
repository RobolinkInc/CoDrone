/*
  CoDrone.cpp - CoDrone library
  Copyright (C) 2014 RoboLink.  All rights reserved.
  LastUpdate : 2017-11-17
*/

#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROMs.h>
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
	unsigned short  makeCRC   = CRC16_Make(data, len + 2);

	if (receiveCRC == makeCRC )	   	crcCheck = true;
	else					  		crcCheck = false;
	return crcCheck;
}

/***************************************************************************/

void CoDroneClass::begin(long baud)
{
	DRONE_SERIAL.begin(baud);   		// µå·Ð°ú Åë½Å °³½Ã	(115200bps)


	#if defined(FIND_HWSERIAL1)
		DEBUG_SERIAL.begin(baud);		// Serial Debug Begin	(115200bps)
		debugMode = 1;					// DEBUG MODE 0 = OFF, 1 = ON
		displayMode = 0;				// LED Display 0 = BOARD LED 0FF, 1 = BOARD LED ON	
	#endif

	SendInterval = 50; 					// millis seconds
	
	analogOffset = 10;					// analog sensor offset
	//HealthTime = millis();
	LED_Start();

	// Connected Drone Address Read
	if (EEPROMs.read(EEP_AddressCheck) == 1)
	{  	
		for (int i = 0; i <= 5; i++)
		{
			devAddressConnected[i] = EEPROMs.read(EEP_AddressFirst+i);
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
	if(action == DiscoverStop)	  	Send_Command(cType_LinkDiscoverStop, 0);		//DiscoverStop
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

//////////////////////////TRIM///////////////////////////////////////////

void CoDroneClass::Set_TrimAll(int _roll, int _pitch, int _yaw, int _throttle, int _wheel)
{
	byte _packet[12];
	byte _crc[2]; 

  	/*
  		header + data
  		header :
  		data type = trimall(_packet[0])
  		data length = 10 (_packet[1])
  	*/

	//header
	_packet[0] = dType_TrimAll;
	_packet[1] = 10;

    //seperate data to high part and low part
	_packet[2]  = _roll & 0xff;
	_packet[3]  = (_roll >> 8) & 0xff;

	_packet[4]  = _pitch & 0xff;
	_packet[5]  = (_pitch >> 8) & 0xff;
	
	_packet[6]  = _yaw & 0xff;
	_packet[7]  = (_yaw >> 8) & 0xff;
	
	_packet[8]  = _throttle & 0xff;
	_packet[9]  = (_throttle >> 8) & 0xff;
	
	_packet[10] = _wheel & 0xff;
	_packet[11] = (_wheel >> 8) & 0xff;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	//send packet arr, length, and crc
	sendCheckFlag = 1;
	Send_Processing(_packet, _packet[1], _crc);
	Send_Check(_packet,_packet[1],_crc);
}



void CoDroneClass::Set_TrimFlight(int _roll, int _pitch, int _yaw, int _throttle)
{	
	byte _packet[10];
	byte _crc[2];

 	/*
  		header + data
  		header :
  		data type = trimflight(_packet[0])
  		data length = 8 (_packet[1])
  	*/

	//header
	_packet[0] = dType_TrimFlight;
	_packet[1] = 8;

    //seperate data to high part and low part
	_packet[2] 	= _roll & 0xff;
	_packet[3] 	= (_roll >> 8) & 0xff;

	_packet[4] 	= _pitch & 0xff;
	_packet[5] 	= (_pitch >> 8) & 0xff;
	
	_packet[6]	= _yaw & 0xff;
	_packet[7] 	= (_yaw >> 8) & 0xff;
	
	_packet[8]  = _throttle & 0xff;
	_packet[9]  = (_throttle >> 8) & 0xff;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	//send packet arr, length, and crc
	sendCheckFlag = 1;
	Send_Processing(_packet,_packet[1],_crc);
	Send_Check(_packet,_packet[1],_crc);

}

void CoDroneClass::Set_TrimDrive(int _wheel)
{	
	byte _packet[4];
	byte _crc[2];
	
	/*
  		header + data
  		data type = trimdrive(_packet[0])
  		data length =  (_packet[1])
  	*/

	//header
	_packet[0]  = dType_TrimDrive;
	_packet[1]  = 2;

    //seperate data to high part and low part
	_packet[2] 	= _wheel & 0xff;
	_packet[3]	= (_wheel >> 8) & 0xff;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	//send packet arr, length, and crc
	sendCheckFlag = 1;
	Send_Processing(_packet,_packet[1],_crc); 
	Send_Check(_packet,_packet[1],_crc);
}

void CoDroneClass::Set_TrimReset()
{
	Set_TrimAll(0,0,0,0,0);	
}
///////////////////////////Send Check//////////////////////////////////

void CoDroneClass::Send_Check(byte _data[], byte _length, byte _crc[])
{	
	/*
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
			Send_Processing(_data,_length,_crc);

		}
		sendCheckFlag = 0;
	}*/
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


///////////////////////CONTROL///////////////////////////////////////////////

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

	/*
  		header + data
  		data type = Control(_packet[0])
  		data length =  (_packet[1])
  	*/   

  	//header
	_packet[0] = dType_Control;
	_packet[1] = 4;

 	 //data
	_packet[2] = roll;
	_packet[3] = pitch;
	_packet[4] = yaw;
	_packet[5] = throttle;

	prevControl[0] = roll;
	prevControl[1] = pitch;
	prevControl[2] = yaw;
	prevControl[3] = throttle;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	//Don't know why they use send check
	sendCheckFlag = 0;
	Send_Processing(_packet,_packet[1],_crc); 
	Send_Check(_packet,_packet[1],_crc);

	roll 		= 0;
	pitch 		= 0;
	yaw 		= 0;
	throttle 	= 0;
}

////////////////send command////////////////////////////////////////

void CoDroneClass::Send_Command(int sendCommand, int sendOption)
{	
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_Command;
	_packet[1] = 0x02;

 	//data
	_packet[2] = sendCommand;
	_packet[3] = sendOption;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;


	Send_Processing(_packet,_packet[1],_crc);
	Send_Check(_packet,_packet[1],_crc);
}


///////////// Bettle ////////////////////////////////////////

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
		weapon = FREE_MISSILE;
		CoDrone.LedColor(ArmHold,  White, 7);
		delay(300);
		CoDrone.LedColor(EyeDimming, White, 7);				
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
			// now done via displayHealth()
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
	
	byte _packet[12];		 
	byte _crc[2];
	

 	//header
	_packet[0] = dType_LedEventCommandIr;
	_packet[1] = 10;

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
	_packet[8]	=	data & 0xff;
	_packet[9]	=	(data >> 8) & 0xff;
	_packet[10]	=	(data >> 16) & 0xff;
	_packet[11]	=	(data >> 24) & 0xff;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	sendCheckFlag = 1;
	Send_Processing(_packet,_packet[1],_crc);  
	Send_Check(_packet,_packet[1],_crc);
	
}

//////////////LED Control/////////////////////////////////////////////////

void CoDroneClass::LedColor(byte sendMode, byte sendColor, byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];

	//header
	_packet[0] = dType_LedMode;
	_packet[1] = 3;

 	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor;
	_packet[4] = sendInterval;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);   
}


void CoDroneClass::LedColor(byte sendMode, byte r, byte g, byte b, byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

 	//data
	_packet[2] = sendMode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = sendInterval;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
}

void CoDroneClass::LedColor(byte sendMode, byte sendColor[], byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];

	//header
	_packet[0] = dType_LedModeColor;
	_packet[1] = 5;

	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2];  
	_packet[6] = sendInterval;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc); 
}


void CoDroneClass::LedEvent(byte sendMode, byte sendColor, byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];

	//header
	_packet[0] = dType_LedEvent;
	_packet[1] = 4;

 	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor;
	_packet[4] = sendInterval;
	_packet[5] = sendRepeat;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);     
}

void CoDroneClass::LedEvent(byte sendMode, byte r, byte g, byte b, byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_LedEvent;
	_packet[1] = 6;

 	//data
	_packet[2] = sendMode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = sendInterval;
	_packet[7] = sendRepeat;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
}

void CoDroneClass::LedEvent(byte sendMode, byte sendColor[], byte sendInterval, byte sendRepeat)
{	
	byte _packet[9];
	byte _crc[2];

	//header
	_packet[0] = dType_LedEvent;
	_packet[1] = 6;

	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2];  
	_packet[6] = sendInterval;
	_packet[7] = sendRepeat;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);     
}


//-170426

void CoDroneClass::LedColorDefault(byte sendMode, byte r, byte g, byte b, byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = sendMode;
	_packet[3] = r;
	_packet[4] = g;  
	_packet[5] = b;  
	_packet[6] = sendInterval;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
}

void CoDroneClass::LedColorDefault(byte sendMode, byte sendColor[], byte sendInterval)
{	
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_LedDefaultColor;
	_packet[1] = 5;

 	//data
	_packet[2] = sendMode;
	_packet[3] = sendColor[0];
	_packet[4] = sendColor[1];  
	_packet[5] = sendColor[2]; 
	_packet[6] = sendInterval;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);
}

//------------------------------//
//edit //170821
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

//////////////////Request//////////////////////////////////////

void CoDroneClass::Request_DroneState()
{
	Send_Command(cType_Request, Req_State);    
}
void CoDroneClass::Request_DroneAttitude()
{
	receiveAttitudeSuccess = 0;
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
	//DEBUG_SERIAL.println("Requesting data:");
	Send_Command(cType_Request, Req_ImuRawAndAngle);    
}
void CoDroneClass::Request_Pressure()
{
	sendCheckFlag = 1;
	Send_Command(cType_Request, Req_Pressure);    
}
void CoDroneClass::Request_ImageFlow()
{
	sendCheckFlag = 1;
	Send_Command(cType_Request, Req_ImageFlow);    
}
void CoDroneClass::Request_Button()
{
	Send_Command(cType_Request, Req_Button);    
}
void CoDroneClass::Request_Battery()
{
	Send_Command(cType_Request, Req_Battery);    
}
void CoDroneClass::Request_Motor()
{
	Send_Command(cType_Request, Req_Motor);    
}
void CoDroneClass::Request_Temperature()
{
	Send_Command(cType_Request, Req_Temperature);    
}

void CoDroneClass::Request_CountFlight()
{
	Send_Command(cType_Request, Req_CountFlight);    
}

void CoDroneClass::Request_CountDrive()
{
	Send_Command(cType_Request, Req_CountDrive);    
}

void CoDroneClass::Request_Range()
{
	receiveRangeSuccess = 0;
	sendCheckFlag = 1;
	Send_Command(cType_Request, Req_Range);    
}



////////////////Print Sensor data////////////////////////////////////////////////////
/*
void CoDroneClass::PrintAttitude(){
	Request_DroneAttitude();
	unsigned long StartCheck = millis();
	while (CoDrone.receiveAttitudeSuccess == 0){
		Receive();
	}
	receiveAttitudeSuccess = 0;

	Send_LinkModeBroadcast(LinkModeMute);    
	
	DRONE_SERIAL.print("\r\n");
	DRONE_SERIAL.print("gyro angle [0] = ");
	DRONE_SERIAL.print(gyroAngle[0]);
	DRONE_SERIAL.print("  gyro angle [1] = ");
	DRONE_SERIAL.print(gyroAngle[1]);
	DRONE_SERIAL.print("  gyro angle [2] = ");
	DRONE_SERIAL.print(gyroAngle[2]);
	DRONE_SERIAL.print("\r\n");

	Send_LinkModeBroadcast(LinkBroadcast_Active);   
	
}
/*
void CoDroneClass::PrintPressure(){  //Not Working
	Request_Pressure();
	unsigned long StartCheck = millis();
	while (CoDrone.receivePressureSuccess == 0){
		Receive();
		if(millis() - StartCheck > 200) break;
	}
	receivePressureSuccess = 0;

	Send_LinkModeBroadcast(LinkModeMute);    
	
	DRONE_SERIAL.print("\r\n");
	DRONE_SERIAL.print("Pressure = ");
	DRONE_SERIAL.print(dronePressure[0]);
	DRONE_SERIAL.print(", ");
	DRONE_SERIAL.print(dronePressure[1]);
	DRONE_SERIAL.print(", ");
	DRONE_SERIAL.print(dronePressure[2]);
	DRONE_SERIAL.print(", ");
	DRONE_SERIAL.print(dronePressure[3]);
	DRONE_SERIAL.print("\r\n");

	Send_LinkModeBroadcast(LinkBroadcast_Active);   
	
}

void CoDroneClass::PrintFlow(){  
	Request_ImageFlow();
	unsigned long StartCheck = millis();
	while (CoDrone.receiveFlowSuccess == 0){
		Receive();
		if(millis() - StartCheck > 200) break;
	}
	receiveFlowSuccess = 0;

	Send_LinkModeBroadcast(LinkModeMute);    
	
	DRONE_SERIAL.print("\r\n");
	DRONE_SERIAL.print("Flow X = ");
	DRONE_SERIAL.print(imageFlowX);
	DRONE_SERIAL.print(",  Flow Y =");
	DRONE_SERIAL.print(imageFlowY);
	DRONE_SERIAL.print("\r\n");

	Send_LinkModeBroadcast(LinkBroadcast_Active);   
	
}
*/

///////////ping///////////////////////////////////////////////////////////

void CoDroneClass::Send_Ping()
{

	byte _packet[10];
	byte _crc[2];

  	//header
	_packet[0] = dType_Ping;
	_packet[1] = 0;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc);  

}

///////////////Connect//////////////////////////////////////////////

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

	  		 if((discoverFlag == 3) && (connectFlag == 0)) //Address find
	  		 {	  		

	  		 	LED_Standard();

	  		 	PreviousMillis = millis();
	  		 	while(!TimeCheck(1000))			
	  			{  
	  		  	   Receive();  
	  		  	}

	  		  	delay(50);
	  		 	discoverFlag = 0;
	  		 	Send_ConnectNearbyDrone();  	  		 				//  Connect Start
	  		 }
	  		 
	  		 //be commented
	  		 /*
	  		 else if (discoverFlag == 4)	// Address not find : re-try
	  		 {
	  		 	delay(50);
	  		 	Send_Discover(DiscoverStart);
	  		 	PreviousMillis = millis();
	  		 }*/

	  		 else
	  		 {	  	
		  		if (TimeCheck(400))		//time out & LED
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

  				if ((discoverFlag == 3) && (connectFlag == 0))	//Address find
  		 		{  	
  		 			LED_Standard();

  		 			PreviousMillis =millis();
  		 			while(!TimeCheck(1000))
  		 			{
  		 				Receive();
  		 			}
					delay(50);
					discoverFlag = 0;
					Send_ConnectConnectedDrone();  	 	//  Connect Start 		 	
  		 		}

  		 		//be commented
  		 		/*
  		 		else if (discoverFlag == 4)	// Address not find : re-try
  		 		{
  		 			Send_Discover(DiscoverStart);
  		 			PreviousMillis = millis();
  		 		}*/

  		 		else
  		 		{	  	
	  				if (TimeCheck(400))  //time out & LED
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

		  		 //be commented 	
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
		//added
		byte AddrCheck3 = 0;
		byte AddrCheck4 = 0;
		
		for (int i = 0; i <= 5; i++)
		{
			if (address[i] == devAddress0[i])  AddrCheck0++;
			if (address[i] == devAddress1[i])  AddrCheck1++;
			if (address[i] == devAddress2[i])  AddrCheck2++;
			//added
			if (address[i] == devAddress3[i])  AddrCheck3++;
			if (address[i] == devAddress4[i])  AddrCheck4++;
		}		
		if(AddrCheck0 == 6)			Send_Connect(0);   
		else if(AddrCheck1 == 6)	Send_Connect(1);   
		else if(AddrCheck2 == 6)	Send_Connect(2);
		//added
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
		//added
		byte AddrCheck3 = 0;
		byte AddrCheck4 = 0;
		
		for (int i = 0; i <= 5; i++)
		{
			if (devAddressConnected[i] == devAddress0[i])  AddrCheck0++;
			if (devAddressConnected[i] == devAddress1[i])  AddrCheck1++;
			if (devAddressConnected[i] == devAddress2[i])  AddrCheck2++;
			//added
			if (devAddressConnected[i] == devAddress3[i])  AddrCheck3++;
			if (devAddressConnected[i] == devAddress4[i])  AddrCheck4++;
		}		
		if(AddrCheck0 == 6)			Send_Connect(0);   
		else if(AddrCheck1 == 6)	Send_Connect(1);   
		else if(AddrCheck2 == 6)	Send_Connect(2);
		//added
		else if(AddrCheck1 == 6)	Send_Connect(3);   
		else if(AddrCheck2 == 6)	Send_Connect(4);
	}	
}

void CoDroneClass::Send_ConnectNearbyDrone()
{
	// old ver.
	/*
	if (devCount > 0)
	{
		if (devRSSI0 > devRSSI1)
		{
			if (devRSSI0 > devRSSI2)	Send_Connect(0);     
			else			Send_Connect(2);
		}
		else
		{
			if (devRSSI1 > devRSSI2)	 Send_Connect(1);
			else	 		Send_Connect(2);
		}
	}
	*/
	//new ver.
	byte highRSSIDrone = 0;	
	int _address[] = {devRSSI0, devRSSI1, devRSSI2, devRSSI3, devRSSI4};
		
	if (devCount > 0)
	{ 	
  		for (int i = 1 ; i < 5 ; i++)
		{
			if (_address[highRSSIDrone] < _address[i]) highRSSIDrone = i;
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
	//should be change to +_length+2
	for(int i = 0; i < _length + 3 ; i++)
	{
		_packet[i+2] = _data[i];	 
	}
  	//CRC  
	_packet[_length + 4] =_crc[1];
	_packet[_length + 5] =_crc[0]; 

	DRONE_SERIAL.write(_packet, _length + 6);


	//debug
	/*
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
	*/

}

////////Receive()///////////////////////////////////////////////////////////////////////

void CoDroneClass::Receive()
{	
	if (DRONE_SERIAL.available() > 0)
	{	
		int input = DRONE_SERIAL.read();
		cmdBuff[cmdIndex++] = (char)input;

		if(cmdIndex < MAX_PACKET_LENGTH)
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
			
			else if (checkHeader == 1)
			{
				if (cmdBuff[1] == START2)	checkHeader = 2;
				else
				{
					checkHeader = 0;
					cmdIndex = 0;
				}
			}
			
			else if (checkHeader == 2)
			{
				if (cmdIndex == 3)
				{
					receiveDtype =  cmdBuff[2];
					dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
				}
	        	else if (receiveDtype != dType_StringMessage) //not message string
	        	{
	        		if (cmdIndex == 4)
	        		{
	        			receiveLength = cmdBuff[3];
	        			dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
	        		}
	        		else if (cmdIndex > 4)
	        		{
	        			if (receiveLength + 5 > cmdIndex)	 	dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];     
		        		//else if (receiveLength + 6 > cmdIndex)	crcBuff[0]  = cmdBuff[cmdIndex - 1];
	        			else if (receiveLength + 6 <= cmdIndex)
	        			{
	        				byte crcBuff[2];

	        				crcBuff[0]  = cmdBuff[cmdIndex - 2];
	        				crcBuff[1]  = cmdBuff[cmdIndex - 1];

	        				
	        				//for(int i = 0; i < receiveLength + 5 ; i++)	dataBuff[i - 3] = cmdBuff[i - 1];

	        				if (CRC16_Check(dataBuff, receiveLength, crcBuff))  	receiveComplete = 1;
	        				else  													receiveComplete = -1;

	        				byte receiveCompleteData[30];

	        				if (receiveComplete == 1)

	        				{                       	
	        					if (receiveDtype == dType_LinkState)		
	        					{
	        						receiveLinkState = dataBuff[2];
	        						receiveLikMode	 = dataBuff[3];                	
	        					}                                	
	        					else if (receiveDtype == dType_LinkEvent)		
	        					{
	        						receiveEventState = dataBuff[2];
	        					}

	        					else if (receiveDtype == dType_Range)			//drone Range
								{ 
								    //left    
									receiveCompleteData[0] = dataBuff[2];
									receiveCompleteData[1] = dataBuff[3];
	                				//front
									receiveCompleteData[2] = dataBuff[4];	
									receiveCompleteData[3] = dataBuff[5];
	                				//right
									receiveCompleteData[4] = dataBuff[6];
									receiveCompleteData[5] = dataBuff[7];
	                				//rear
									receiveCompleteData[6] = dataBuff[8];
									receiveCompleteData[7] = dataBuff[9];
	                				//top
									receiveCompleteData[8] = dataBuff[10];
									receiveCompleteData[9] = dataBuff[11];
	                				//bottom
									receiveCompleteData[10] = dataBuff[12];                	
									receiveCompleteData[11] = dataBuff[13];   
								}

								else if (receiveDtype == dType_CountFlight)		//drone CountFlight
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
	                		        
								}     
								else if (receiveDtype == dType_CountDrive)		//drone CountDrive
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
								//--------------------------------------------------------------------

								else if (receiveDtype ==  dType_Button)			//drone Button
				                {
				                	receiveCompleteData[0] = dataBuff[2];
				                }

				                else if (receiveDtype == dType_TrimDrive)		//drone TrimDrive
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

				                //drone Attitude or dron GyroBias
				                else if (receiveDtype == dType_Attitude || receiveDtype == dType_GyroBias)		
				                { 
				                	receiveCompleteData[0] = dataBuff[2];
				                	receiveCompleteData[1] = dataBuff[3];
				                	receiveCompleteData[2] = dataBuff[4];
				                	receiveCompleteData[3] = dataBuff[5];
				                	receiveCompleteData[4] = dataBuff[6];
				                	receiveCompleteData[5] = dataBuff[7];
				                }                            

	            			    else if (receiveDtype == dType_State)			//drone state
	            			    {
	            			    	//ModeDrone
	                				receiveCompleteData[0] = dataBuff[2];
	                				//Modevehicle
	         				       	receiveCompleteData[1] = dataBuff[3];
	         				       	//ModeFlight
	     				           	receiveCompleteData[2] = dataBuff[4];
	     				           	//Modedrive
	               				 	receiveCompleteData[3] = dataBuff[5];
	               				 	//SensorOrientation
				                	receiveCompleteData[4] = dataBuff[6];
				                	//Coordinate
				                	receiveCompleteData[5] = dataBuff[7];
				                	//Battery
				                	receiveCompleteData[6] = dataBuff[8];	  	              
				                }
				                
				                //drone TrimFlight or drone ImageFlow or dron Temperature
				                else if (receiveDtype == dType_TrimFlight ||receiveDtype ==  dType_ImageFlow ||receiveDtype ==  dType_Temperature)		
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
				                
				                else if (receiveDtype == dType_ImuRawAndAngle)//drone ImuRawAndAngle
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
				                	receiveCompleteData[16] = dataBuff[18]; 
				                	receiveCompleteData[17] = dataBuff[19]; 
					            }

				                else if (receiveDtype == dType_TrimAll)		//drone TrimAll
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

				                //drone Pressure or drone Battery or drone Motor
				                else if (receiveDtype == dType_Pressure ||receiveDtype ==  dType_Battery || receiveDtype == dType_Motor)
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
				                
				                else if (receiveDtype == dType_LinkRssi)			//Discovered Device
				                {
				                	rssi = dataBuff[2];
				                	rssi = rssi - 256;
									//debug
									/*
				                	#if defined(FIND_HWSERIAL1)
				                		if(debugMode == 1)
				                		{		
					                		DEBUG_SERIAL.print("RSSI : ");	
					                		DEBUG_SERIAL.println(rssi);									  
					                	}
									#endif  
									*/
				                }
				                
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
				                		//LED_PORTC(0b00000010);
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
				                		//LED_PORTC(0b00000110);
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
				                	//added
				                	
				                	else if (devIndex == 3)
				                	{
				                		for (int i = 3; i <= 8; i++)
				                		{
				                			devAddress3[i - 3] = dataBuff[i];
				                		}

				                		for (int i = 9; i <= 28; i++)
				                		{
				                			devName2[i - 3] = dataBuff[i];
				                		}

				                		devRSSI3 = dataBuff[29];
				                		devFind[3] = 1;   
				                		//LED_PORTC(0b00100110);
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
				                		//LED_PORTC(0b00100110);
				                	}
									devCount = devFind[0] + devFind[1] + devFind[2] + devFind[1] + devFind[2];
				                	


	                				//devCount = devFind[0] + devFind[1] + devFind[2];
	                				
	                				// debug
	                  				/*#if defined(FIND_HWSERIAL1)
	                					if(debugMode == 1)
					                	{								  	
					                		//DisplayAddress(devCount, devName0, devName1, devName2, devName3, devName4); //Address display
					                		DisplayAddress(devCount, devName0, devName1, devName2);
					                	}
								    #endif  */
	                			}
            				}                   

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
		else
		{
			checkHeader = 0;
			cmdIndex = 0;
		}
	}
}


/////////////Print Address //////////////////////////////////////////////////////

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


//debug
void CoDroneClass::DisplayAddress(byte count, byte _devName0[], byte _devName1[],byte _devName2[], byte _devName3[], byte _devName4[])
{	
  #if defined(FIND_HWSERIAL1)
	if(debugMode == 1)
	{																			  	
		if (count == 1)			DEBUG_SERIAL.print("index 0 - ADRESS : ");
		else if (count == 2)	DEBUG_SERIAL.print("index 1 - ADRESS : ");
		else if (count == 3)	DEBUG_SERIAL.print("index 2 - ADRESS : ");
		//added
		else if (count == 4)	DEBUG_SERIAL.print("index 4 - ADRESS : ");
		else if (count == 5)	DEBUG_SERIAL.print("index 5 - ADRESS : ");
		
		for (int i = 0; i <= 5; i++)
		{
			if (count == 1)			DEBUG_SERIAL.print(devAddress0[i], HEX);
			else if (count == 2)	DEBUG_SERIAL.print(devAddress1[i], HEX); 
			else if (count == 3)	DEBUG_SERIAL.print(devAddress2[i], HEX); 
			//added
			else if (count == 4)	DEBUG_SERIAL.print(devAddress3[i], HEX); 
			else if (count == 5)	DEBUG_SERIAL.print(devAddress4[i], HEX); 
			if(i < 5)	DEBUG_SERIAL.print(", ");
		}
		DEBUG_SERIAL.print("\t");

		DEBUG_SERIAL.print("NAME :");

		for (int i = 0; i <= 19; i++)
		{
			if (count == 1)			DEBUG_SERIAL.write(devName0[i]);	    
			else if (count == 2)	DEBUG_SERIAL.write(devName1[i]);
			else if (count == 3)	DEBUG_SERIAL.write(devName2[i]);
			//added
			else if (count == 4)	DEBUG_SERIAL.write(devName3[i]);
			else if (count == 5)	DEBUG_SERIAL.write(devName4[i]);
			
		}	  	  	  

		DEBUG_SERIAL.print(" RSSI : ");

		if (count == 1)     	DEBUG_SERIAL.println(devRSSI0 - 256);
		else if (count == 2)	DEBUG_SERIAL.println(devRSSI1 - 256);
		else if (count == 3)	DEBUG_SERIAL.println(devRSSI2 - 256);
		//added
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

			if (_rssi < 0 )_rssi = 0;
			if (_rssi > 7 )_rssi = 7;

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
	//linkState = -1;
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
	if(receiveComplete > 0)
	{
		if (receiveDtype == dType_State)
		{
			if (droneState[0] != 0 )
			{	
				droneState[0] = _completeData[0];
				droneState[1] = _completeData[1];
				droneState[2] = _completeData[2];
				droneState[3] = _completeData[3];
				droneState[4] = _completeData[4];
				droneState[5] = _completeData[5];	
				droneState[6] = _completeData[6]; 		  
			}		
		}
			
		else if (receiveDtype == dType_IrMessage)		//IrMessage
		{
	   		byte irMassageDirection	= _completeData[0];
	   		irMassageReceive	= ((_completeData[4] << 24) | (_completeData[3] << 16) | (_completeData[2] << 8) | (_completeData[1] & 0xff));			
	   	}                          
	   	
	   	else if (receiveDtype == dType_Attitude)
		{  	
			/*		
			attitudeRoll	= droneAttitude[0]; //((droneAttitude[1] << 8) | (droneAttitude[0]  & 0xff));
			attitudePitch	= droneAttitude[1]; //((droneAttitude[3] << 8) | (droneAttitude[2]  & 0xff))
			attitudeYaw		= droneAttitude[2]; //((droneAttitude[5] << 8) | (droneAttitude[4]  & 0xff));
			*/
			attitudeRoll	= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			attitudePitch	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			attitudeYaw		= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			receiveAttitudeSuccess = 1;														  	
		}	 
		
		else if(receiveDtype == dType_GyroBias)
		{
			GyroBias_Roll	= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			GyroBias_Pitch	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			GyroBias_Yaw	= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
		}

		else if (receiveDtype == dType_TrimAll)
		{				
			TrimAll_Roll		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			TrimAll_Pitch		= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			TrimAll_Yaw			= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			TrimAll_Throttle	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
			TrimAll_Wheel		= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
		}

		else if (receiveDtype == dType_TrimFlight)
		{				
			TrimAll_Roll		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			TrimAll_Pitch		= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			TrimAll_Yaw			= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			TrimAll_Throttle	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
		}

		else if (receiveDtype == dType_TrimDrive)
		{				
			TrimAll_Wheel		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
		}

		else if(receiveDtype == dType_ImuRawAndAngle)
		{
			ImuAccX	= (_completeData[3] << 8) | (_completeData[2]);// x and y are switched
			ImuAccY	= (_completeData[1] << 8) | (_completeData[0]);// x and y are switched
			ImuAccZ	= -(_completeData[5] << 8) | (_completeData[4]);// y needs to be flipped to have gravity be negative
							
			ImuGyroRoll		= (_completeData[7] << 8) | (_completeData[6]);;
			ImuGyroPitch	= (_completeData[9] << 8) | (_completeData[8]);
			ImuGyroYaw		= (_completeData[11] << 8) | (_completeData[10]);	
									
			ImuAngleRoll	= (_completeData[13] << 8) | (_completeData[12]);
			ImuAnglePitch	= (_completeData[15] << 8) | (_completeData[14]);
			ImuAngleYaw		= (_completeData[17] << 8) | (_completeData[16]);
		}

	  	else if(receiveDtype == dType_Pressure)
	  	{
	  		d1			= ((_completeData[3] << 32)  |(_completeData[2] << 16)  |(_completeData[1] << 8)  | (_completeData[0]  & 0xff));
			d2			= ((_completeData[7] << 32)  |(_completeData[6] << 16)  |(_completeData[5] << 8)  | (_completeData[4]  & 0xff));			  			
			temperature	= ((_completeData[11] << 32) |(_completeData[10] << 16) |(_completeData[9] << 8)  | (_completeData[8]  & 0xff));
			pressure	= ((_completeData[15] << 32) |(_completeData[14] << 16) |(_completeData[13] << 8) | (_completeData[12]  & 0xff));	  						  			
	  		//receivePressureSuccess = 1;	
	  	}
	  	
	  	else if (receiveDtype ==  dType_ImageFlow)
	  	{
	  		fVelocitySumX	= ((_completeData[3] << 32)  |(_completeData[2] << 16)  |(_completeData[1] << 8)  | (_completeData[0]  & 0xff));
	  		fVelocitySumY	= ((_completeData[7] << 32)  |(_completeData[6] << 16)  |(_completeData[5] << 8)  | (_completeData[4]  & 0xff));
	  		//receiveFlowSuccess = 1;
	  	}
		
		else if (receiveDtype == dType_Button)
		{
			button = _completeData[0];
		}

		else if (receiveDtype == dType_Battery)
		{
			Battery_v30			= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			Battery_v33			= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));			  	 	
			Battery_gradient	= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			Battery_yIntercept	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
			flagBatteryCalibration	= _completeData[8];			  	
			Battery_Raw			= ((_completeData[12] << 32) |(_completeData[11] << 16) |(_completeData[10] << 8) | (_completeData[9]  & 0xff));			  
			Battery_Percent		= _completeData[13];			  	
			Battery_voltage		= ((_completeData[15] << 8) | (_completeData[14]  & 0xff));
		}

		else if (receiveDtype == dType_Range)
		{
			sensorRange[0] = ((_completeData[1] << 8) | (_completeData[0]  & 0xff));	//left
			sensorRange[1] = ((_completeData[3] << 8) | (_completeData[2]  & 0xff));	//front
			sensorRange[2] = ((_completeData[5] << 8) | (_completeData[4]  & 0xff));	//right
			sensorRange[3] = ((_completeData[7] << 8) | (_completeData[6]  & 0xff));	//rear
			sensorRange[4] = ((_completeData[9] << 8) | (_completeData[8]  & 0xff));	//top
			sensorRange[5] = ((_completeData[11] << 8)| (_completeData[10]  & 0xff));	//bottom
			receiveRangeSuccess = 1;
		}

		else if (receiveDtype == dType_CountDrive)
		{
			//_completeData[0] to _completeData[7] is drive time (64bits)
			countAccident	= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
		}

		else if (receiveDtype == dType_CountFlight)
		{
			//_completeData[0] to _completeData[7] is flight time (64bits)
			countTakeOff	= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
			countLanding	= ((_completeData[11] << 8) | (_completeData[10]  & 0xff));			  	 	
			countAccident	= ((_completeData[13] << 8) | (_completeData[12]  & 0xff));

		}

		else if (receiveDtype == dType_Motor)
		{
			//forward mean CW way and reverse mean CCW
			m1_forward	= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
			m1_reverse	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
			  	 	
			m2_forward	= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));
			m2_reverse	= ((_completeData[7] << 8) | (_completeData[6]  & 0xff));
			  	 	
			m3_forward	= ((_completeData[9] << 8) | (_completeData[8]  & 0xff));
			m3_reverse	= ((_completeData[11] << 8) | (_completeData[10]  & 0xff));
			  	 		 				  	 	
			m4_forward	= ((_completeData[13] << 8) | (_completeData[12]  & 0xff));
			m4_reverse	= ((_completeData[15] << 8) | (_completeData[14]  & 0xff));
		}

		else if (receiveDtype == dType_Temperature)
		{
			imu_temp		= ((_completeData[3] << 32)  |(_completeData[2] << 16)  |(_completeData[1] << 8)  | (_completeData[0]  & 0xff));
			pressure_temp	= ((_completeData[7] << 32)  |(_completeData[6] << 16)  |(_completeData[5] << 8)  | (_completeData[4]  & 0xff));
		}

	  	else if (receiveDtype == dType_LinkState)
	  	{				
	  		linkState = receiveLinkState;
	  	}		

	  	else if ((receiveDtype == dType_LinkEvent) && (receiveEventState > 0))
	  	{  	
	  		if (receiveEventState == linkEvent_Scanning)
	  		{
	  			if(discoverFlag == 1)		discoverFlag = 2;
	  		}
	  		else if (receiveEventState == linkEvent_ScanStop)
	  		{
	  			if(discoverFlag == 2)
	  			{
	  				/*
	  				if(devCount > 0)		discoverFlag = 3;
	  				else					discoverFlag = 4;*/
	  				discoverFlag = 3;
	  			}	    
	  		}
	  		else if (receiveEventState == linkEvent_ReadyToControl)
	  		{	    	
	  			if(connectFlag == 1)
	  			{
	  				connectFlag = 0;         
		  			EEPROMs.write(EEP_AddressCheck, 0x01);						
		  			for (int i = 0; i <= 5; i++)
		  			{
						//	devAddressConnected[i] = devAddressBuf[i];
		  				EEPROMs.write(EEP_AddressFirst + i, devAddressBuf[i]);				
		  			}
		  		}			  
		  		LED_Connect();
		  		pairing = true;    
		  		delay(500);
		  	}
		  	else if (receiveEventState == linkEvent_RspWriteSuccess)
		  	{
		  		if(sendCheckFlag == 1)		sendCheckFlag = 2;			  		
		    }
		  	else if (receiveEventState == linkEvent_Write)
			{	
				if(sendCheckFlag == 2)		sendCheckFlag = 3;
			}
		}
		
		receiveEventState = -1;	  
		receiveComplete = -1;
		receiveLength = -1;
		receiveLinkState = -1;
		receiveDtype = -1;	
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
	boolean checktime = false;
	unsigned long currentMillis = millis();
	if (currentMillis - PreviousMillis > interval)
	{
		PreviousMillis = currentMillis;
		checktime = true;
	}
	return checktime;
}

boolean CoDroneClass::TimeOutSendCheck(word interval) //milliseconds
{

	boolean timeout = false;
	unsigned long currentMillis = millis();
	if (currentMillis - timeOutSendPreviousMillis > interval)
	{
		timeOutSendPreviousMillis = currentMillis;
		timeout = true;
	}
	return timeout;
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

	int length = 1000/tempo;  					//TEMPO
	long delayValue = 1000000/frequency/2; 		 // calculate the delay value between transitions
	long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
	
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

int CoDroneClass::GestureCheckPosition(int sen1, boolean act1, int sen2, boolean act2)
{
	boolean state = false;
	int val1,val2;
	boolean ck1, ck2;
	
	val1 = analogRead(sen1);
	val2 = analogRead(sen2);	
	
	//---------------------------------------------------------------------------------------//         	
	if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);		// ¡ãUP
	else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ¡ådown
	if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);		// ¡ãUP	
	else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ¡ådown	
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

//- ex // if (CoDrone.gestureFuntion(A4, LOW, A6, LOW, &CoDroneClass::FlightEvent, Stop)); 
int CoDroneClass::GestureFuntion(int sen1, boolean act1, int sen2, boolean act2,void(CoDroneClass::*pf)(byte),byte command)
{	
	boolean state = false;
	int val1,val2;
	boolean ck1, ck2;
	
	val1 = analogRead(sen1);
	val2 = analogRead(sen2);	
	
	//---------------------------------------------------------//         	
	if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);			// ¡ãUP
	else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ¡ådown
	if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);			// ¡ãUP	
	else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ¡ådown	
	//---------------------------------------------------------//
	
	if(ck1 && ck2)
	{
		while (ck1 && ck2)
		{				
			(CoDrone.*pf)(command);
			delay(50);
			
			val1 = analogRead(sen1);
			val2 = analogRead(sen2);	 
			         	
			if(act1 == HIGH)	ck1 = (val1 > JOY_UP_LIMIT);			// ¡ãUP
			else				ck1 = (val1 < JOY_DOWN_LIMIT);		// ¡ådown
			if(act2 == HIGH)	ck2 = (val2 > JOY_UP_LIMIT);			// ¡ãUP	
			else				ck2 = (val2 < JOY_DOWN_LIMIT);		// ¡ådown						
		}	
		
		while (ck1 || ck2)	//return
	  {
			val1 = analogRead(sen1);
			val2 = analogRead(sen2);	 
			         	
			if(act1 == HIGH)	ck1 = (val1 > JOY_UP_RETURN_LIMIT);		// ¡ãUP
			else				ck1 = (val1 < JOY_DOWN_RETURN_LIMIT);	// ¡ådown
			if(act2 == HIGH)	ck2 = (val2 > JOY_UP_RETURN_LIMIT);		// ¡ãUP	
			else				ck2 = (val2 < JOY_DOWN_RETURN_LIMIT);	// ¡ådown	
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


CoDroneClass CoDrone;                         
