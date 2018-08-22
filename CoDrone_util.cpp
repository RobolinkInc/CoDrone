#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>

//-------------------------------------------------------------------------------------------------------//
//----------------------------------	Link board mode change---------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
//----------------------------------------	CRC checksum	-----------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
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
	for (int counter = 0; counter < len; counter++)	crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ * (char *)buf++) & 0x00FF];
	return crc;
}

boolean CoDroneClass::CRC16_Check(unsigned char data[], int len, unsigned char crc[])
{
	boolean crcCheck = false;
	unsigned short receiveCRC = ((crc[1] << 8) | (crc[0]  & 0xff));
	unsigned short makeCRC = CRC16_Make(data, len + 2);
	if (receiveCRC == makeCRC)				crcCheck = true;
	return crcCheck;
}

//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//--------------------------------- Smart Inventor LED Display ------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::LED_Display(byte led_mode, int value)	//for inventor board
{
	if(displayMode == 1)
	{
		if(led_mode == LED_DISPLAY_PORTC)						PORTC |= value;
		else if(led_mode == LED_DISPLAY_DDRC)				DDRC |= value;
		else if(led_mode == LED_DISPLAY_MOVE_RADER)	PORTC = (0b1 << value) | (0b10000000 >> value);

		else if(led_mode == LED_DISPLAY_START)
		{
			LED_Display(LED_DISPLAY_MOVE_SLIDE, 0);
			LED_Display(LED_DISPLAY_STANDARD, 0);
		}
		else if(led_mode == LED_DISPLAY_STANDARD)
		{
			DDRC 	=	0b01100110;
			PORTC = 0b00000000;
		}
		else if(led_mode == LED_DISPLAY_MOVE_SLIDE)
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

//-------------------------------------------------------------------------------------------------------//
//-------------------------------- Smart Inventor Sensor Read -------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::ButtonPressHoldWait(int button)
{
	do {
	delay(10);
	}	while (digitalRead(button));
}

void CoDroneClass::ButtonPressHoldWait(int button1, int button2)
{
	do {
	delay(10);
	}	while (digitalRead(button1) && digitalRead(button2));
}

int CoDroneClass::AnalogScaleChange(int analogValue)
{
	int ScaleChange = map(analogValue, 0, 1023, -100, 100);
	if ((ScaleChange > -1 * analogOffset) && (ScaleChange < analogOffset)) ScaleChange = 0;
	return ScaleChange;
}
int CoDroneClass::scaleChange(int analogValue)
{
	int ScaleChange = map(analogValue, 0, 1023, -100, 100);
	if ((ScaleChange > -1 * analogOffset) && (ScaleChange < analogOffset)) ScaleChange = 0;
	return ScaleChange;
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//------------------------------------------ Buzz -------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::Buzz(long frequency, int tempo)
{
	DDRD |= 0b10000000;

	int length = 1000 / tempo;									// TEMPO
	long delayValue = 1000000 / frequency / 2;	// calculate the delay value between transitions
	long numCycles = frequency * length / 1000;	// calculate the number of cycles for proper timing

	for (long i=0; i < numCycles; i++)
	{
		PORTD |= 0b10000000;
		while(!TimeCheckBuzz(delayValue));
		PORTD &=~0b10000000;
		while(!TimeCheckBuzz(delayValue));
	}
}
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
//----------------------------------------- Data Display ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::PrintDroneAddress()
{
	delay(500);
	Send_LinkModeBroadcast(LinkBroadcast_Mute);
	delay(100);
	DRONE_SERIAL.println();
	DRONE_SERIAL.println("//----------------- Connected Drone Address -----------------//");
	DRONE_SERIAL.print("byte droneAddress[6] = {");		
	for(char i = 0; i <= 4; i++)
	{
		DRONE_SERIAL.print("0x");
		DRONE_SERIAL.print(devAddressConnected[i],HEX);
		DRONE_SERIAL.print(", ");
	}
	DRONE_SERIAL.print("0x");
	DRONE_SERIAL.print(devAddressConnected[5],HEX);
	
	DRONE_SERIAL.println("};");
	DRONE_SERIAL.println("CoDrone.AutoConnect(AddressInputDrone, droneAddress);");
	DRONE_SERIAL.println("//------------------------------------------------------------//");
}

void CoDroneClass::DisplayRSSI()
{
	if(displayMode == 1)
	{
		//Send_RSSI_Polling(PollingStart);
		Send_Command(cType_LinkRssiPollingStart, 0x02);  	//RssiPollingStart	(0x02 * 100 = 200ms)
		delay(300);
		DDRC = 0xff;
		PORTC = 0x00;
		while(1)
		{
			Receive();
			int _rssi = abs(RSSI);
			_rssi = (_rssi / 10) -2;
			if (_rssi < 0 )_rssi = 0;
			else if (_rssi > 7) _rssi = 7;
			PORTC = 0b1;
			for (int i = 0; i < _rssi ; i++)	PORTC |= PORTC << 1;
		}
	}
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//------------------------------------ LOW Battery Check ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
int CoDroneClass::LowBatteryCheck(byte value)
{
	int bat = -1;
	bat = getBatteryVoltage();
	if(bat < value)
	{
		for (int i = 0; i < 5; i++)
		{
			CoDrone.Buzz(2000, 4);
			delay(400);
		}
	}
	return bat;
}
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//------------------------------------------- TimeCheck -------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

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

//------------getter and setter-----------------------

/*
 *	description :	all these function for getter and setter  
 *				 	set value to variable or return the variable value
 *	
 *	getter function
 *	praram 		: 	none
 *	return 		:	
 *					- roll 		: int
 *					- pitch 	: int
 *					- yaw 		: int
 *					- throttle 	: int 
 *
 *	setter fuction
 *	param 		:
 *					- roll 		: int
 *					- pitch 	: int
 *					- yaw 		: int
 *					- throttle 	: int 
 *	return 		:	 none
 */

void CoDroneClass::setRoll(int _roll)
{
	roll = _roll;
}

int CoDroneClass::getRoll()
{
	return roll;
}

void CoDroneClass::setPitch(int _pitch)
{
	pitch = _pitch;
}
int CoDroneClass::getPitch()
{
	return pitch;
}

void CoDroneClass::setYaw(int _yaw)
{
	yaw = _yaw;
}

int CoDroneClass::getYaw(){
	return yaw;
}

void CoDroneClass::setThrottle(int _throttle){
	throttle = _throttle;
}

int CoDroneClass::getThrottle(){
	return throttle;
}
//-------------------------------------------------------------------------------------------------------//
