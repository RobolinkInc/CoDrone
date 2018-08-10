/*
	CoDrone.cpp	-	CoDrone	library
	Copyright	(C)	2014 RoboLink.	All	rights reserved.
	LastUpdate : 2018-05-31
*/

#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>

CoDroneClass CoDrone;

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Begin ----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::begin(long	baud)
{
	DRONE_SERIAL.begin(baud);							// 드론과	통신 개시	(115200bps)
	#if	defined(FIND_HWSERIAL1)
		DEBUG_SERIAL.begin(baud);						// Serial	Debug	Begin	(115200bps)
		displayMode	=	0;										// LED Display 0 = BOARD LED 0FF,	1	=	BOARD	LED	ON
	#endif
	SendInterval = 50;										// millis	seconds
	analogOffset = 10;										// analog	sensor offset
	LED_Display(LED_DISPLAY_START, 0);		// LED_Start();
	if (EEPROM.read(EEP_AddressCheck))		// Connected Drone Address Read
	{
		for	(int i = 0;	i	<= 5;	i++)	devAddressConnected[i] = EEPROM.read(EEP_AddressFirst+i);
		isConnectedBefore = true;	
	}	
	#if	!defined(__AVR_ATmega328PB__)
		Send_LinkModeBroadcast(LinkModeActive);			//Link Active	Mode
		delay(100);
	#endif
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//---------------------------------------- Discover Drone -----------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::Send_Discover(byte	action)
{
	if(action	== DiscoverStop)				Send_Command(cType_LinkDiscoverStop, 0);
	else if(action ==	DiscoverStart)
	{
		Send_Command(cType_LinkDiscoverStart,	0);
		discoverFlag = 1;
	}
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//----------------------------------------- AutoConnect ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//


void CoDroneClass::AutoConnect()								// NearbyDrone or ConnectedDrone
{
	pinMode(10, INPUT_PULLUP);    								//DipSw3
  if (!digitalRead(10))	 isConnectedBefore = false;		//DipSw3 down ↓
  
  if(isConnectedBefore)	AutoConnect(ConnectedDrone);  
  else AutoConnect(NearbyDrone);  
}
void CoDroneClass::AutoConnect(byte	mode)
{
	byte _temp[0];
	ConnectionProcess(mode, _temp);
}

void CoDroneClass::AutoConnect(byte	mode,	byte address[])
{
	ConnectionProcess(mode, address);
}
void CoDroneClass::pair()
{
	byte _temp[0];
	if(isConnectedBefore)
		ConnectionProcess(ConnectedDrone, _temp);
	else
		ConnectionProcess(NearbyDrone, _temp);
}
void CoDroneClass::pair(int mode)
{
	byte _temp[0];
	if(mode == Nearest)	ConnectionProcess(NearbyDrone, _temp);
}
void CoDroneClass::pair(byte address[])
{
	ConnectionProcess(AddressInputDrone, address);
}
void CoDroneClass::ConnectionProcess(byte	mode,	byte address[])
{
	unsigned long currentTimes;
	byte displayLED = 0;
	//-----------------------------------------------------------//
	if (getState() != 0)		pairing	=	true;
	//-----------------------------------------------------------//
	else																			// AutoConnect start
	{
		Send_Discover(DiscoverStart);
		PreviousMillis = millis();
		LED_Display(LED_DISPLAY_DDRC,	0xff);
		unsigned long tryconnect = millis();
		unsigned long buzzerTime = millis();

		while(!pairing)
		{
			if( millis() > buzzerTime+4000)
			{
				buzzerTime = millis();
				CoDrone.Buzz(550, 16);
				delay(100);
				CoDrone.Buzz(550, 8);
			} 
			if((discoverFlag ==	3) &&	(connectFlag ==	0))	// Address find
			{
				LED_Display(LED_DISPLAY_STANDARD,	0);
				PreviousMillis = millis();

				while(!TimeCheck(1000))					Receive();
				while (DRONE_SERIAL.available() > 0)     Receive();

				discoverFlag = 0;

				if (devCount > 0)
				{
					if 		(mode == NearbyDrone)			Send_ConnectDrone(mode, address);
					else if (mode == ConnectedDrone)	 	Send_ConnectDrone(mode, devAddressConnected);
					else if (mode == AddressInputDrone)		Send_ConnectDrone(mode, address);
				}
			}
			else if((!(connectFlag == 1)) && (TimeCheck(400)))	// time	out	&	LED
			{
				if (displayLED++ ==	4)
				{
					displayLED = 0;
					delay(50);
					Send_Discover(DiscoverStart);
				}
				LED_Display(LED_DISPLAY_MOVE_RADER,	displayLED);
				PreviousMillis = millis();
			}
		  while (DRONE_SERIAL.available() > 0) 	Receive();
		}
		delay(50);
	}
	CoDrone.Buzz(700, 16);
  	CoDrone.Buzz(900, 16);
  	CoDrone.Buzz(1050, 8);
	LED_Connect();
	
  while (DRONE_SERIAL.available() > 0)     Receive();  
  delay(50);  
  roll = 0;	pitch = 0;	yaw = 0;	throttle = 0;
  Control();
  delay(50);  
  while (DRONE_SERIAL.available() > 0)     Receive();  
  delay(50);     
}

void CoDroneClass::Send_ConnectDrone(byte mode, byte address[])
{
	byte connectDevice = 5;
	
	if(mode == NearbyDrone)
	{
		byte highRSSIDrone = 0;

		for	(int i = 0 ; i < 5 ; i++)
		{
			if (devRSSI[highRSSIDrone]	>	devRSSI[i]) highRSSIDrone = highRSSIDrone;
			else highRSSIDrone = i;
		}
		connectDevice = highRSSIDrone;
	}
	else if((mode == ConnectedDrone) || (mode == AddressInputDrone))
	{
		byte _addrCk0 = 0,_addrCk1 = 0,_addrCk2 = 0, _addrCk3 = 0, _addrCk4 = 0;
		byte _rvAddrCk0 = 0, _rvAddrCk1 = 0, _rvAddrCk2 = 0, _rvAddrCk3 = 0, _rvAddrCk4 = 0;		//reverse addr check - robolinl-bt

		for	(int i = 0;	i	<= 5;	i++)		// same	address	check
		{
			if (address[i] ==	devAddress0[i])	 	_addrCk0++;
			if (address[i] ==	devAddress1[i])	 	_addrCk1++;
			if (address[i] ==	devAddress2[i])	 	_addrCk2++;
			if (address[i] ==	devAddress3[i])	 	_addrCk3++;
			if (address[i] ==	devAddress4[i])	 	_addrCk4++;

			if (address[i] ==	devAddress0[5-i])	_rvAddrCk0++;
			if (address[i] ==	devAddress1[5-i])	_rvAddrCk1++;
			if (address[i] ==	devAddress2[5-i])	_rvAddrCk2++;
			if (address[i] ==	devAddress3[5-i])	_rvAddrCk3++;
			if (address[i] ==	devAddress4[5-i])	_rvAddrCk4++;
		}
		if	   ((_addrCk0 == 6)		|| (_rvAddrCk0	== 6))	connectDevice = 0;
		else if((_addrCk1 ==	6)	|| (_rvAddrCk1	== 6))	connectDevice = 1;
		else if((_addrCk2 ==	6)	|| (_rvAddrCk2	== 6))	connectDevice = 2;
		else if((_addrCk3 ==	6)	|| (_rvAddrCk3	== 6))	connectDevice = 3;
		else if((_addrCk4 ==	6)	|| (_rvAddrCk4	== 6))	connectDevice = 4;
	}

	if(connectDevice < 5)
	{
		connectFlag	=	1;
		for	(int i = 0;	i	<= 5;	i++)
		{
			if(connectDevice ==	0)			devAddressBuf[i] = devAddress0[i];
			else if(connectDevice	== 1)	devAddressBuf[i] = devAddress1[i];
			else if(connectDevice	== 2)	devAddressBuf[i] = devAddress2[i];
			else if(connectDevice	== 3)	devAddressBuf[i] = devAddress3[i];
			else if(connectDevice	== 4)	devAddressBuf[i] = devAddress4[i];
		}

		#if	defined(DEBUG_MODE_ENABLE)
			DEBUG_SERIAL.println(connectDevice,	HEX);
		#endif

		Send_Command(cType_LinkConnect,	connectDevice);
	}	
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//------------------------------------------ Receive ----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::Receive()
{
	if (DRONE_SERIAL.available() > 0)
	{
		int	input	=	DRONE_SERIAL.read();

		#if	defined(DEBUG_MODE_ENABLE)
		//	DEBUG_SERIAL.print(input,HEX);	DEBUG_SERIAL.print(" ");
		#endif
		cmdBuff[cmdIndex++]	=	(char)input;

		if (cmdIndex >=	MAX_PACKET_LENGTH)
		{
			checkHeader	=	0;
			cmdIndex = 0;
		}
		else
		{
			if (cmdIndex ==	1)
			{
				if (cmdBuff[0] ==	START1)	checkHeader	=	1;
				else
				{
					checkHeader	=	0;
					cmdIndex = 0;
				}
			}
			else if	((cmdIndex	== 2)	&& (checkHeader	== 1))
			{
				if (cmdBuff[1] ==	START2)	checkHeader	=	2;
				else
				{
					checkHeader	=	0;
					cmdIndex = 0;
				}
			}
			else if	(checkHeader ==	2)
			{
				if (cmdIndex ==	3)	receiveDtype = cmdBuff[2];		// Receive Dtype

				else if	(receiveDtype	!= dType_StringMessage)			// not message	string
				{
					if (cmdIndex ==	4)	receiveLength	=	cmdBuff[3];	// Receive Length

					else if	(cmdIndex	>	4)
					{
						if (receiveLength	+	6	<= cmdIndex)
						{
							byte crcBuff[2];
							crcBuff[0] =	cmdBuff[cmdIndex - 2];
							crcBuff[1] =	cmdBuff[cmdIndex - 1];

							byte dataBuff[30];
							for(int	i	=	0; i < receiveLength + 5 ; i++)	dataBuff[i - 3]	=	cmdBuff[i	-	1];

							byte receiveComplete = 0;
							if (CRC16_Check(dataBuff,	receiveLength, crcBuff))	receiveComplete	=	1;

							byte receiveCompleteData[28];

							if (receiveComplete	== 1)
							{
								if	((receiveDtype ==	dType_Pressure)			||	(receiveDtype ==	 dType_Battery)			||	(receiveDtype	==	dType_Motor)				||	(receiveDtype	== dType_ImuRawAndAngle)||
									 (receiveDtype ==	dType_CountFlight)		||	(receiveDtype	== dType_Range)				||	(receiveDtype ==	dType_TrimAll)				||	(receiveDtype	== dType_CountDrive)	||
									 (receiveDtype ==	dType_TrimFlight)		||	(receiveDtype ==	 dType_ImageFlow)		||	(receiveDtype ==	 dType_Temperature)			||	(receiveDtype	== dType_State)			||
									 (receiveDtype ==	dType_Attitude)			||	(receiveDtype ==	dType_GyroBias)			||	(receiveDtype	== dType_IrMessage)				||	(receiveDtype	== dType_TrimDrive)		||
									 (receiveDtype	==	dType_Button)			||	(receiveDtype ==	dType_Ack)				||	(receiveDtype ==	dType_LinkDiscoveredDevice)	||	(receiveDtype ==	dType_LinkState)	||
									 (receiveDtype ==	dType_LinkEvent)		||	(receiveDtype	== dType_LinkEventAddress)	||	(receiveDtype ==	dType_LinkRssi))	
									 {
										for(byte i = 0; i <= 27; i++)	receiveCompleteData[i] = dataBuff[i + 2];
									 }
							}
							checkHeader	=	0;
							cmdIndex = 0;
							if(receiveComplete)	ReceiveEventCheck(receiveCompleteData);
						}
					}
				}
				else
				{
					checkHeader	=	0;
					cmdIndex = 0;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//---------------------------------- ReceiveEventCheck --------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::ReceiveEventCheck(byte	_completeData[])
{
	if (receiveDtype	== dType_LinkDiscoveredDevice)//Discovered Device
	{
		byte devIndex	=	_completeData[0];
		byte devName[20];

		
		for	(int i = 1;	i	<= 6;	i++)
		{
			 if			(devIndex == 0)		devAddress0[i-1] = _completeData[i];
			 else if 	(devIndex == 1)		devAddress1[i-1] = _completeData[i];
			 else if 	(devIndex == 2)		devAddress2[i-1] = _completeData[i];
			 else if 	(devIndex == 3)		devAddress3[i-1] = _completeData[i];
			 else if 	(devIndex == 4)		devAddress4[i-1] = _completeData[i];
		}


		devRSSI[devIndex]	=	_completeData[27];
		devFind[devIndex]	=	1;
		devCount = devFind[0]	+	devFind[1] + devFind[2]	+	devFind[3] + devFind[4];
	}

	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype ==	dType_State)
	{

		droneState[0]	=	_completeData[0];
		droneState[1]	=	_completeData[1];
		droneState[2]	=	_completeData[2];
		droneState[3]	=	_completeData[3];
		droneState[4]	=	_completeData[4];
		droneState[5]	=	_completeData[5];
		droneState[6]	=	_completeData[6];

		receiveStateSuccess = 1;

	}
	 //---------------------------------------------------------------------------------------------//

	else if	(receiveDtype	== dType_LinkRssi)
	{
		rssi = _completeData[0];
		rssi = rssi	-	256;
	}

	else if	(receiveDtype == dType_Ack)
	{
	}

	else if	(receiveDtype	== dType_IrMessage)
	{
			byte irMassageDirection	=	_completeData[0];

			unsigned long	_irMessge[4];

			_irMessge[0] = _completeData[1];
			_irMessge[1] = _completeData[2];
			_irMessge[2] = _completeData[3];
			_irMessge[3] = _completeData[4];

			irMessageReceive	=	((_irMessge[3] <<	24)	|	(_irMessge[2]	<< 16) | (_irMessge[1] <<	8) | (_irMessge[0]	&	0xff));

	 }
	//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_Attitude)
	{
		attitudeRoll	=	((_completeData[1] <<	8) | (_completeData[0]	&	0xff));
		attitudePitch	=	((_completeData[3] <<	8) | (_completeData[2]	&	0xff));
		attitudeYaw		=	((_completeData[5] <<	8) | (_completeData[4]	&	0xff));

		receiveAttitudeSuccess = 1;

	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_GyroBias)
	{
		int GyroBias_Roll		= ((_completeData[1] << 8) | (_completeData[0]  & 0xff));
		int GyroBias_Pitch	= ((_completeData[3] << 8) | (_completeData[2]  & 0xff));
		int GyroBias_Yaw		= ((_completeData[5] << 8) | (_completeData[4]  & 0xff));

		receiveGyroSuccess = 1;

	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_TrimAll)
	{
		TrimAll_Roll			=	((_completeData[1] <<	8) | (_completeData[0]	&	0xff));
		TrimAll_Pitch			=	((_completeData[3] <<	8) | (_completeData[2]	&	0xff));
		TrimAll_Yaw				=	((_completeData[5] <<	8) | (_completeData[4]	&	0xff));
		TrimAll_Throttle	=	((_completeData[7] <<	8) | (_completeData[6]	&	0xff));
		TrimAll_Wheel			=	((_completeData[9] <<	8) | (_completeData[8]	&	0xff));

	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_TrimFlight)		//
	{
		TrimAll_Roll		=	((_completeData[1] <<	8) | (_completeData[0]	&	0xff));
		TrimAll_Pitch		=	((_completeData[3] <<	8) | (_completeData[2]	&	0xff));
		TrimAll_Yaw			=	((_completeData[5] <<	8) | (_completeData[4]	&	0xff));
		TrimAll_Throttle	=	((_completeData[7] <<	8) | (_completeData[6]	&	0xff));

		receiveTrimSuccess = 1;

	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_TrimDrive)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if(receiveDtype ==	dType_ImuRawAndAngle)
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

		receiveAccelSuccess = 1;

	}

//---------------------------------------------------------------------------------------------//

	else if(receiveDtype ==	dType_Pressure)
	{
		long d1			= ((_completeData[3] << 32)  |(_completeData[2] << 16)  |(_completeData[1] << 8)  | (_completeData[0]  & 0xff));
		long d2			= ((_completeData[7] << 32)  |(_completeData[6] << 16)  |(_completeData[5] << 8)  | (_completeData[4]  & 0xff));
		temperature	= ((_completeData[11] << 32) |(_completeData[10] << 16) |(_completeData[9] << 8)  | (_completeData[8]  & 0xff));
		pressure		= ((_completeData[15] << 32) |(_completeData[14] << 16) |(_completeData[13] << 8) | (_completeData[12]  & 0xff));

		receivePressureSuccess = 1;

	}
	//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	==	dType_ImageFlow)
	{
		fVelocitySumX	= ((_completeData[3] << 32)  |(_completeData[2] << 16)  |(_completeData[1] << 8)  | (_completeData[0]  & 0xff));
		fVelocitySumY	= ((_completeData[7] << 32)  |(_completeData[6] << 16)  |(_completeData[5] << 8)  | (_completeData[4]  & 0xff));

		receiveOptSuccess = 1;

	}
	//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_Button)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_Battery)
	{
		int	batteryV30											=	((_completeData[1] <<	8) | (_completeData[0]	&	0xff));
		int	batteryV33											=	((_completeData[3] <<	8) | (_completeData[2]	&	0xff));
		int	batteryGradient								=	((_completeData[5] <<	8) | (_completeData[4]	&	0xff));
		int	batteryyIntercept							=	((_completeData[7] <<	8) | (_completeData[6]	&	0xff));
		int	flagBatteryCalibration					=	_completeData[8];
		int	batteryRaw											=	((_completeData[12]	<< 32) |(_completeData[11] <<	16)	|(_completeData[10]	<< 8)	|	(_completeData[9]	 & 0xff));
		batteryPercent											=	_completeData[13];
		batteryVoltage											=	((_completeData[15]	<< 8)	|	(_completeData[14]	&	0xff));

		receiveBatterySuccess = 1;

	}

	else if	(receiveDtype	==	dType_Range)
	{
		sensorRange[0] = ((_completeData[1]	<< 8)	|	(_completeData[0]	 & 0xff));	//left
		sensorRange[1] = ((_completeData[3]	<< 8)	|	(_completeData[2]	 & 0xff));	//front
		sensorRange[2] = ((_completeData[5]	<< 8)	|	(_completeData[4]	 & 0xff));	//right
		sensorRange[3] = ((_completeData[7]	<< 8)	|	(_completeData[6]	 & 0xff));	//rear
		sensorRange[4] = ((_completeData[9]	<< 8)	|	(_completeData[8]	 & 0xff));	//top
		sensorRange[5] = ((_completeData[11] <<	8)| (_completeData[10]	 & 0xff));	//bottom

		receiveRangeSuccess	=	1;

	}
	else if	(receiveDtype	==	dType_CountDrive)
	{
	}

	else if	(receiveDtype	== dType_CountFlight)
	{
	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	==	dType_Motor)
	{
	}
//---------------------------------------------------------------------------------------------//
	else if	(receiveDtype	== dType_Temperature)
	{
	}

 //---------------------------------------------------------------------------------------------//

	else if	(receiveDtype	== dType_LinkState)
	{
		byte	receiveLinkState	=	_completeData[0];
	//	receiveLikMode		=	_completeData[1];

		if(receiveLinkState	== linkMode_None)
		{
		}
		else if(receiveLinkState ==	linkMode_Boot)
		{
		}
		else if(receiveLinkState ==	linkMode_Ready)
		{
		}
		else if(receiveLinkState ==	linkMode_Connecting)
		{
		}
		else if(receiveLinkState ==	linkMode_Connected)
		{
		}
		else if(receiveLinkState ==	linkMode_Disconnecting)
		{
		}
		else if(receiveLinkState ==	linkMode_ReadyToReset)
		{
		}
		linkState	=	receiveLinkState;
	}
//---------------------------------------------------------------------------------------------//

	else if	((receiveDtype ==	dType_LinkEvent) ||	(receiveDtype	== dType_LinkEventAddress))
	{
		byte	receiveEventState	=	_completeData[0];

		
//-----------------------------------------------------------------------------------------------//

		if (receiveEventState	== linkEvent_ScanStop)
		{
			if((discoverFlag ==	1) ||	(discoverFlag	== 2))	discoverFlag = 3;
			
		}

		else if	(receiveEventState ==	linkEvent_Connected)
		{
			if(connectFlag ==	1)
			{
				connectFlag	=	0;
				EEPROM.write(EEP_AddressCheck, 0x01);
				for	(int i = 0;	i	<= 5;	i++)		EEPROM.write(EEP_AddressFirst	+	i, devAddressBuf[i]);
			}
			pairing	=	true;
			delay(500);
		}

		else if	(receiveEventState ==	linkEvent_ReadyToControl)
		{
			if(connectFlag ==	1)
			{
				connectFlag = 0;
				EEPROM.write(EEP_AddressCheck, 0x01);
				for	(int i = 0;	i	<= 5;	i++)		EEPROM.write(EEP_AddressFirst	+	i, devAddressBuf[i]);
			}
			pairing	=	true;
			delay(500);
		}

		else if	(receiveEventState ==	linkEvent_RspWriteSuccess)
		{
			if(sendCheckFlag ==	1)	sendCheckFlag	=	2;
		}

		else if	(receiveEventState ==	linkEvent_Write)
		{
			if(sendCheckFlag ==	2)	sendCheckFlag	=	3;
		}

		else if	(receiveEventState ==	linkEvent_SystemReset)
		{
		}

		else if	(receiveEventState ==	linkEvent_Initialized)
		{
		}

		else if	(receiveEventState ==	linkEvent_Scanning)
		{
			if(discoverFlag	== 1)	discoverFlag = 2;
		}

//----------------------------------------------------------------------//

		else if	(receiveEventState ==	linkEvent_None)
		{
		}

		else if	(receiveEventState ==	linkEvent_FoundDroneService)
		{
		}

		else if	(receiveEventState ==	linkEvent_Connecting)
		{
		}

		else if	(receiveEventState ==	linkEvent_ConnectionFaild)
		{
		}

		else if	(receiveEventState ==	linkEvent_ConnectionFaildNoDevices)
		{
		}

		else if	(receiveEventState ==	linkEvent_ConnectionFaildNotReady)
		{
		}

		else if	(receiveEventState ==	linkEvent_PairingStart)
		{
		}

		else if	(receiveEventState ==	linkEvent_PairingSuccess)
		{
		}

		else if	(receiveEventState ==	linkEvent_PairingFaild)
		{
		}

		else if	(receiveEventState ==	linkEvent_BondingSuccess)
		{
		}

		else if	(receiveEventState ==	linkEvent_LookupAttribute)
		{
		}

		else if	(receiveEventState ==	linkEvent_RssiPollingStart)
		{
		}

		else if	(receiveEventState ==	linkEvent_RssiPollingStop)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverService)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverCharacteristic)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverCharacteristicDroneData)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverCharacteristicDroneConfig)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverCharacteristicUnknown)
		{
		}

		else if	(receiveEventState ==	linkEvent_DiscoverCCCD)
		{
		}

		else if	(receiveEventState ==	linkEvent_Disconnecting)
		{
		}

		else if	(receiveEventState ==	linkEvent_Disconnected)
		{
		}

		else if	(receiveEventState ==	linkEvent_GapLinkParamUpdate)
		{
		}

		else if	(receiveEventState ==	linkEvent_RspReadError)
		{
		}

		else if	(receiveEventState ==	linkEvent_RspReadSuccess)
		{
		}

		else if	(receiveEventState ==	linkEvent_RspWriteError)
		{
		}

		else if	(receiveEventState ==	linkEvent_SetNotify)
		{
		}
	}
}
//-------------------------------------------------------------------------------------------------------//0