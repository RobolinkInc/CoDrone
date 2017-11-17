/*
  CoDrone.h - CoDrone library
  Copyright (C) 2014 RoboLink.  All rights reserved.
  LastUpdate : 2016-04-20
*/

#ifndef CoDrone_h
#define CoDrone_h
#include "Arduino.h"
//#include <SoftwareSerial.h>
#include <avr/interrupt.h>


/***********************************************************************/


/***********************************************************************/
//////////////////////////typedef///////////////////////////////////////
/***********************************************************************/

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/***********************************************************************/
////////////////////////Serial Select////////////////////////////////////
/***********************************************************************/
#if defined(UBRRH) || defined(UBRR0H)
#define FIND_HWSERIAL0
#endif

#if defined(UBRR1H)
#define FIND_HWSERIAL1
#endif

#if defined (FIND_HWSERIAL1)		//Serial Other Setting
#define DRONE_SERIAL 	Serial1		//drone serial
#define DEBUG_SERIAL    Serial		//debug serial1

#else								//Serial Smart Setting
#define DRONE_SERIAL 	Serial		//drone serial	
#define DEBUG_SERIAL    Serial1		//debug serial1

#endif

/***********************************************************************/
////////////////////////////HEADER///////////////////////////////////////
/***********************************************************************/
//START CODE
#define START1    	0x0A
#define START2   	0x55

/***********************************************************************/

#define MAX_PACKET_LENGTH 	100

/***********************************************************************/
#define BATTLE_CHECK_TIME  		500
#define LED_CHECK_TIME 			60
#define	SEND_CHECK_TIME    		10
#define RECEIVE_CHECK_TIME      10

/***********************************************************************/

#define ROLL							CoDrone.roll
#define PITCH							CoDrone.pitch
#define YAW								CoDrone.yaw
#define THROTTLE						CoDrone.throttle
#define STATE							CoDrone.state
#define SEND_INTERVAL					CoDrone.SendInterval
#define ANALOG_OFFSET					CoDrone.analogOffset
#define BATTERY							CoDrone.battery
#define RSSI							CoDrone.rssi

#define AttitudeROLL				CoDrone.attitudeRoll
#define AttitudePITCH				CoDrone.attitudePitch
#define AttitudeYAW					CoDrone.attitudeYaw


/***********************************************************************/

#define DiscoverStop  			cType_LinkDiscoverStop
#define DiscoverStart  			cType_LinkDiscoverStart

#define PollingStop				cType_LinkRssiPollingStop
#define PollingStart			cType_LinkRssiPollingStart

#define	PAIRING					CoDrone.pairing

#define LinkModeMute 			LinkBroadcast_Mute
#define LinkModeActive			LinkBroadcast_Active
#define LinkModePassive 		LinkBroadcast_Passive

#define	NearbyDrone    			1
#define	ConnectedDrone  		2
#define AddressInputDrone 		3

//eeprom address
#define	EEP_AddressCheck   		10
#define	EEP_AddressFirst  		11
#define	EEP_AddressEnd  		15

/////////////////////////////////////////////////////////////////////////

#define FREE_PLAY				0
#define TEAM_RED				1
#define TEAM_BLUE				2
#define TEAM_GREEN				3
#define TEAM_YELLOW				4


/**********************	IR DATA****************************************/

#define FREE_MISSILE			0xaa01
#define RED_MISSILE				0xbb01
#define BLUE_MISSILE			0xcc01
#define GREEN_MISSILE			0xdd01
#define YELLOW_MISSILE			0xee01

/***********************************************************************/

#define Flight 						dMode_Flight
#define FlightNoGuard				dMode_FlightNoGuard,
#define FlightFPV					dMode_FlightFPV
#define Drive 				 		dMode_Drive
#define DriveFPV					dMode_DriveFPV

#define Absolute 					cSet_Absolute
#define Relative		 			cSet_Relative

#define TakeOff 					fEvent_TakeOff
#define FlipFront					fEvent_FlipFront
#define FlipRear					fEvent_FlipRear
#define FlipLeft					fEvent_flipLeft
#define FlipRight					fEvent_FlipRight
#define Stop						fEvent_Stop
#define Landing						fEvent_Landing
#define TurnOver					fEvent_TurnOver
#define Shot						fEvent_Shot
#define UnderAttack					fEvent_UnderAttack
#define Square						fEvent_Square
#define CircleLeft					fEvent_CircleLeft
#define CircleRight					fEvent_CircleRight
#define Rotate180					fEvent_Rotate180

#define RollIncrease			trim_RollIncrease
#define RollDecrease			trim_RollDecrease
#define PitchIncrease			trim_PitchIncrease
#define PitchDecrease			trim_PitchDecrease
#define YawIncrease				trim_YawIncrease
#define YawDecrease				trim_YawDecrease
#define ThrottleIncrease		trim_ThrottleIncrease
#define ThrottleDecrease		trim_ThrottleDecrease

#define DataAvailable			(DRONE_SERIAL.available() > 0)

/***********************************************************************/
/////////////////////////LINK MODULE/////////////////////////////////////
/***********************************************************************/
enum ModeLink
{
	linkMode_None = 0,	 	// none
	linkMode_Boot,	 	 	//boot
	linkMode_Ready,	 		// ready (before connect)
	linkMode_Connecting,	// connecting
	linkMode_Connected,	 	// connected
	linkMode_Disconnecting,	// during disconnecting
	linkMode_ReadyToReset,	// wait reset(reset after 1sec)
	linkMode_EndOfType
};

enum ModeLinkBroadcast
{
	LinkBroadcast_None = 0, // none
	LinkBroadcast_Mute,     // stop data sending - yellow LED dimming
	LinkBroadcast_Active, 	// send request data and every event
	LinkBroadcast_Passive, 	// send request data and important event
	LinkBroadcast_EndOfType
};

enum EventLink
{
	linkEvent_None = 0,								// none
		
	linkEvent_SystemReset,							// system reset
		
	linkEvent_Initialized,							// finish board init
		
	linkEvent_Scanning,								// start scanning
	linkEvent_ScanStop,								// stop scanning

	linkEvent_FoundDroneService,				    // success scanning, drone service

	linkEvent_Connecting,							// start connect to drone
	linkEvent_Connected,							// finish connect

	linkEvent_ConnectionFaild,					    // connection fail
	linkEvent_ConnectionFaildNoDevices,	            // connection fail - no device
	linkEvent_ConnectionFaildNotReady,	            // connection fail - drone is not ready

	linkEvent_PairingStart,							// start pairing
	linkEvent_PairingSuccess,						// pairing success
	linkEvent_PairingFaild,							// pairing faild

	linkEvent_BondingSuccess,						// bonding success

	linkEvent_LookupAttribute,					    // search service and status(start GATT Event)

	linkEvent_RssiPollingStart,					    // start RSSI Polling
	linkEvent_RssiPollingStop,					    // stop RSSI polling

	linkEvent_DiscoverService,						// search(discover) service
	linkEvent_DiscoverCharacteristic,				// search(discover) characteristic
	linkEvent_DiscoverCharacteristicDroneData,		// search(discover) characteristic
	linkEvent_DiscoverCharacteristicDroneConfig,	// search(discover) characteristic
	linkEvent_DiscoverCharacteristicUnknown,		// search(discover) characteristic
	linkEvent_DiscoverCCCD,				            // search(discover) CCCD

	linkEvent_ReadyToControl,			            // ready to control

	linkEvent_Disconnecting,			            // start disconnecting
	linkEvent_Disconnected,				            // disconnecting done

	linkEvent_GapLinkParamUpdate,	                // GAP_LINK_PARAM_UPDATE_EVENT

	linkEvent_RspReadError,				            // RSP read error
	linkEvent_RspReadSuccess,			            // RSP read success

	linkEvent_RspWriteError,			            // RSP write error
	linkEvent_RspWriteSuccess,		                // RSP write success

	linkEvent_SetNotify,					        // Notify activate
	linkEvent_Write,							    // data write event

	EndOfType
};


/***********************************************************************/
//////////////////////////////DRONE/////////////////////////////////////
/***********************************************************************/
enum DataType
{
	dType_None = 0, 					// none
	
	// ½Ã½ºÅÛ Á¤º¸
	dType_Ping, 						// check connetion(reserved)
	dType_Ack, 							// response about get data
	dType_Error, 						// error(reserved)
	dType_Request, 						// request specific data type
	dType_DeviceName, 			    	//
	
	// Á¶Á¾, ¸í·É
	dType_Control = 0x10, 		        // control
	dType_Command, 						// single command
	dType_Command2, 					// multi command(change 2 status in same time)
	DType_Command3, 					// multi command(change 3 status in same time)
	
	// LED
	dType_LedMode = 0x20, 		        // set Single LED mode
	dType_LedMode2, 					// set 2 LED mode
	dType_LedModeCommand,        		// LED mode, command
	dType_LedModeCommandIr, 	        // LED mode, command, sending IR data
	dType_LedModeColor, 	    		// LED mode¸ set single RGB color 
	dType_LedModeColor2, 	    		// LED mode¸ set double RGB color
	dType_LedEvent, 					// LED single event
	dType_LedEvent2, 					// LED double event,
	dType_LedEventCommand, 	        	// LED event, command
	dType_LedEventCommandIr,	        // LED event, command, sending IR data
	dType_LedEventColor, 			    // LED event set single RGB color
	dType_LedEventColor2,       		// LED event set double RGB color
	
	// status
	dType_Address = 0x30, 		        // IEEE address
	dType_State, 						// drone's status(flight mode, coordinate, battery level)
	dType_Attitude, 					// drone's attitude (Vector)
	dType_GyroBias,						// gyro bias value (Vector)
	dType_TrimAll, 						// Trim (roll, pitch, yaw, throttle, wheel)
	dType_TrimFlight,					// flight mode trim (roll, pitch, yaw, throttle)
	dType_TrimDrive, 					// drive mode trim (wheel)
			
	// data communication
	dType_IrMessage = 0x40, 			// IR data communication
		
	// sensor and control
	dType_ImuRawAndAngle = 0x50, 	    // IMU Raw + Angle
	dType_Pressure, 					// pressure sensor data
	dType_ImageFlow, 					// ImageFlow
	dType_Button, 					    // button value
	dType_Batery, 						// battery
	dType_Motor, 						// motor control, check motor contol value
	dType_Temperature, 					// Temperature
	
	// Link module
	dType_LinkState = 0xE0,				// link module state
	dType_LinkEvent,					// link module event
	dType_LinkEventAddress,				// link module event + address
	dType_LinkRssi,						// value of RSSI between link and connected device
	dType_LinkDiscoveredDevice, 		// discovered devices
	dType_LinkPasscode,                	// set necessary Passcode when pairing
	dType_StringMessage = 0xD0,     	//
	dType_EndOfType
};

/***********************************************************************/
enum CommandType
{
	cType_None = 0, 						// none
	
	// setting
	cType_ModeDrone = 0x10, 				// change drone mode
	
	// control
	cType_Coordinate = 0x20, 				// change coordinate
	cType_Trim, 							// change trim
	cType_FlightEvent, 						// run flight event
	cType_DriveEvent, 						// run drive event
	cType_Stop, 							// stop
	cType_ResetHeading = 0x50, 			    // reset direction
	cType_ClearGyroBiasAndTrim, 		    // clear gyro sensor data and trim
	
	// Åë½Å
	cType_PairingActivate = 0x80, 	        //
	cType_PairingDeactivate, 	    		//
	cType_TerminateConnection,   			//

	// request
	cType_Request = 0x90, 					// request specific data type
	
	// CODRONE(PETRONE) LINK
	cType_LinkModeBroadcast = 0xE0,         // change LINK(link board) send/receive mode
	cType_LinkSystemReset, 					// restart system
	cType_LinkDiscoverStart, 				// start searching device
	cType_LinkDiscoverStop, 				// stop searching device
	cType_LinkConnect, 						// connect device that user selected
	cType_LinkDisconnect, 					// disconnect
	cType_LinkRssiPollingStart, 	    	// start collecting RSSI
	cType_LinkRssiPollingStop, 		    	///stop collecting RSSI

	cType_EndOfType
};

/***********************************************************************/
enum ModeDrone
{
	dMode_None = 0, 			// none
	dMode_Flight = 0x10,    	// flight mode(include guard)
	dMode_FlightNoGuard,    	// flight mode(none guard)
	dMode_FlightFPV, 			// flight mode(FPV)
	dMode_Drive = 0x20,     	// drive mode
	dMode_DriveFPV, 			// drive mode(FPV)
	dMode_Test = 0x30, 	    	// test mode
	dMode_EndOfType
};

/***********************************************************************/
enum ModeVehicle //actual mode is system
{
	vMode_None = 0,
	vMode_Boot, 				// booting
	vMode_Wait, 				// wait connectable status
	vMode_Ready, 				// ready
	vMode_Running, 				// running main code
	vMode_Update, 				// firmware update
	vMode_UpdateComplete,	    // complete firmware update
	vMode_Error, 				// error
	vMode_EndOfType
};

/***********************************************************************/
enum ModeFlight
{
	fMode_None = 0,
	fMode_Ready, 				// ready for flight
	fMode_TakeOff, 				// take off(change to Flight status automatically)
	fMode_Flight, 				// flight
	fMode_Flip, 				// rotate(flip)
	fMode_Stop, 				// forced stop
	fMode_Landing, 				// landing
	fMode_Reverse, 				// reverse
	fMode_Accident, 			// accident(change Ready status automatically)
	fMode_Error, 				// error
	fMode_EndOfType
};

/***********************************************************************/
enum ModeDrive
{
	dvMode_None = 0,
	dvMode_Ready, 				// ready
	dvMode_Start, 				// drive start
	dvMode_Drive, 				// drive
	dvMode_Stop, 				// forced stop
	dvMode_Accident, 			// accident(change Ready status automatically)
	dvMode_Error, 				// error
	dvMode_EndOfType
};

/***********************************************************************/
enum SensorOrientation
{
	senOri_None = 0,
	senOri_Normal, 				// normal
	senOri_ReverseStart, 	    // start reverse
	senOri_Reverse, 			// reversed
	senOri_EndOfType
};

/***********************************************************************/
enum Coordinate
{
	cSet_None = 0, 				// none
	cSet_Absolute, 				// static coordinate(Absolute position)
	cSet_Relative, 				// normal coordinate(relative position)
	cSet_EndOfType
};

/***********************************************************************/

enum Trim
{
	trim_None = 0,
	trim_RollIncrease, 			// Roll increase
	trim_RollDecrease, 			// Roll decrease
	trim_PitchIncrease, 		// Pitch increase
	trim_PitchDecrease, 		// Pitch decrease
	trim_YawIncrease, 			// Yaw increase
	trim_YawDecrease, 			// Yaw decrease
	trim_ThrottleIncrease, 	    // Throttle increase
	trim_ThrottleDecrease, 	    // Throttle decrease
	trim_EndOfType
};

/***********************************************************************/

enum FlightEvent
{
	fEvent_None = 0,
	fEvent_TakeOff, 			// take off
	fEvent_FlipFront, 		    // flip(not yet)
	fEvent_FlipRear, 			// flip(not yet)
	fEvent_flipLeft, 			// flip(not yet)
	fEvent_FlipRight, 	    	// flip(not yet)
	fEvent_Stop, 				// forced stop
	fEvent_Landing, 			// landing
	fEvent_TurnOver, 			//
	fEvent_Shot, 				//
	fEvent_UnderAttack, 	    //
	fEvent_Square, 				//
	fEvent_CircleLeft, 	    	//
	fEvent_CircleRight,     	//
	fEvent_Rotate180,			//
	fEvent_EndOfType
};

enum DriveEvent
{
	dEvent_None = 0,
	dEvent_Ready, 				//
	dEvent_Start, 				//
	dEvent_Drive, 				//
	dEvent_Stop, 				//
	dEvent_Accident, 			//
	dEvent_Error, 				//
	dEvent_EndOfType
};

/***********************************************************************/
enum Request
{		
	// »óÅÂ
	Req_Address = 0x30, 				    // device(IEEE) address
	Req_State, 								// state (flight mode, coordinate, battery level)
	Req_Attitude, 							// drone's attitude(Vector)
	Req_GyroBias, 							// gyro bias value(Vector)
	Req_TrimAll, 							// trim all (roll, pitch, yaw, throttle, wheel)
	Req_TrimFlight, 						// flight trim (roll, pitch, yaw, throttle)
	Req_TrimDrive, 							// drive trim (wheel)
		
	// ¼¾¼­
	Req_ImuRawAndAngle = 0x50, 	            // IMU Raw + Angle
	Req_Pressure, 							// pressure sensor value
	Req_ImageFlow, 							// ImageFlow sensor
	Req_Button, 							// botton value
	Req_Batery, 							// battery level
	Req_Motor, 								// 4 Motors value
	Req_Temperature, 						// temperature
	Req_EndOfType
};

/***********************************************************************/
enum ModeLight
{
  Light_None,
  WaitingForConnect, 					//
  Connected,
  
  EyeNone = 0x10,
  EyeHold, 								// Eye's LED hold one color
  EyeMix, 								// Eye's LED change color in order
  EyeFlicker, 							// Eye's LED flicker
  EyeFlickerDouble, 					// Eye's LED repeat duoble flicker and stop same time
  EyeDimming, 							// Eye's LED slow flicking
  
  ArmNone = 0x40,
  ArmHold, 								// arm's LED hold one color
  ArmMix, 								// arm's LED change color in order
  ArmFlicker, 							// arm's LED flicker
  ArmFlickerDouble, 					// arm's LED repeat duoble flicker and stop same time
  ArmDimming, 							// arm's LED slow flicking
  ArmFlow, 								// arm's LED flow light forward to backward
  ArmFlowReverse, 						// arm's LEDf low light backward to forward
  EndOfLedMode
};


/***********************************************************************/
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

/***********************************************************************/

class CoDroneClass
{
public:

	CoDroneClass(void);

/////////////////////////////////////////////////////////////////////////

	void begin(long baud);
	
	void Receive(void);
	
	void Send_Control();
	void Control();
	void Control(int interval);

	void Send_Command(int sendCommand, int sendOption);	
	void Send_Processing(byte _data[], byte _length, byte _crc[]);
	
/////////////////////////////////////////////////////////////////////////

	void LinkReset();
	void Send_LinkState();
	void Send_LinkModeBroadcast(byte mode);
	
/////////////////////////////////////////////////////////////////////////
	
	void AutoConnect(byte mode);
  	void AutoConnect(byte mode, byte address[]);	
	void Send_ConnectAddressInputDrone(byte address[]);
	void Send_ConnectConnectedDrone();
	void Send_ConnectNearbyDrone();	
	void Send_Connect(byte index);
	void Send_Disconnect();		
	void Send_Discover(byte action);
	void Send_Check(byte _data[], byte _length, byte _crc[]);

/////////////////////////////////////////////////////////////////////////

	void Send_Ping();
	void Send_ResetHeading();			
	void Send_RSSI_Polling(byte action);	
	void Send_DroneMode(byte event);
	void Send_Coordinate(byte mode);	
	void Send_ClearGyroBiasAndTrim();		
	void DroneModeChange(byte event);			
	void FlightEvent(byte event);
	void DriveEvent(byte event);
	
/////////////////////////////////////////////////////////////////////////

	void BattleShooting();
	void BattleReceive();
	void BattleBegin(byte teamSelect);	
	void BattleDamageProcess();	
	void BattleHitPoints(int points);
	void CrashCustom(boolean custom);
	boolean CrashedCheck();
	void displayHealth();
	
/////////////////////////////////////////////////////////////////////////
		
	void Request_DroneState();	
	void Request_DroneAttitude();
	void Request_DroneGyroBias();	
	
	void Request_TrimAll();
	void Request_TrimFlight();
	void Request_TrimDrive();	
	
	void Request_ImuRawAndAngle();
	void Request_Pressure();
	void Request_ImageFlow();
	void Request_Button();	
	void Request_Battery();	
	void Request_Motor();	
	void Request_Temperature();

/////////////////////////////////////////////////////////////////////////
	void PrintGyro();
	void PrintPressure();
	void PrintFlow();

/////////////////////////////////////////////////////////////////////////

	void Set_Trim(byte event);
	void Set_TrimReset();
	void Set_TrimAll(int _roll, int _pitch, int _yaw, int _throttle, int _wheel);
	void Set_TrimFlight(int _roll, int _pitch, int _yaw, int _throttle);
	void Set_TrimDrive(int _wheel);

/////////////////////////////////////////////////////////////////////////

	void LedColor(byte sendMode, byte sendColor, byte sendInterval);
	void LedColor(byte sendMode, byte r, byte g, byte b, byte sendInterval);
	void LedColor(byte sendMode, byte sendColor[], byte sendInterval);
	
	void LedEvent(byte sendMode, byte sendColor, byte sendInterval, byte sendRepeat);
	void LedEvent(byte sendMode, byte sendColor[], byte sendInterval, byte sendRepeat);
	void LedEvent(byte sendMode, byte r, byte g, byte b, byte sendInterval, byte sendRepeat);
	
/////////////////////////////////////////////////////////////////////////
			
	void LinkStateCheck();
	void ReceiveEventCheck();
	int LowBatteryCheck(byte value);
	void DisplayRSSI();
	
/////////////////////////////////////////////////////////////////////////

	void LED_PORTC(int value);
	void LED_DDRC(int value);
	void LED_Move_Radar(byte display);
	void LED_Start();
	void LED_Move_Slide();
	void LED_Connect();
	void LED_Standard();
	void LED_Blink(int time, int count);
	
/////////////////////////////////////////////////////////////////////////

	unsigned short CRC16_Make(unsigned char *buf, int len); //CRC16-CCITT Format
	boolean CRC16_Check(unsigned char data[], int len, unsigned char crc[]);
	
/////////////////////////////////////////////////////////////////////////

	void PrintDroneAddress();	
	void DisplayAddress(byte count);
	
	void ReadSensor(void);
	void PrintSensor(void);
	
/////////////////////////////////////////////////////////////////////////
	
	void ButtonPreesHoldWait(int button);
	void ButtonPreesHoldWait(int button1, int button2);	
	
	int AnalogScaleChange(int analogValue);			

/////////////////////////////////////////////////////////////////////////

	boolean TimeCheck(word interval); 						//milliseconds
	boolean TimeCheck1(word interval); 						//milliseconds
	boolean TimeCheck2(word interval); 						//milliseconds
	boolean TimeCheck3(word interval); 						//milliseconds
	boolean TimeOutSendCheck(word interval); 				//milliseconds		
	boolean TimeCheckBuzz(word interval); 					//microseconds
	
/////////////////////////////////////////////////////////////////////////

	void Buzz(long frequency, int tempo);
	void BeepWarning(int count);

/////////////////////////////////////////////////////////////////////////

	byte cmdBuff[MAX_PACKET_LENGTH];
	byte dataBuff[MAX_PACKET_LENGTH];
	byte crcBuff[2];
	
	byte checkHeader;
	int cmdIndex;
	int receiveDtype;
	int receiveLength;
	int receiveEventState;
	int receiveLinkState;
	int receiveLikMode;
	int receiveComplete;
	int receiveCRC;
			
/////////////////////////////////////////////////////////////////////////

	byte displayMode;	//smar inventor : default 1
	byte debugMode;		//smar inventor : default 0
	
	byte discoverFlag;
	byte connectFlag;
			
	boolean pairing;
		
	int analogOffset;
	byte displayLED;
	int SendInterval; 	//millis seconds
	byte timeOutRetry;
	
	byte sendCheckFlag;
	
	byte receiveAttitudeSuccess;
	byte receivePressureSuccess;
	byte receiveFlowSuccess;
	
	int energy;
	int MAX_ENERGY = 8;
	boolean CustomCrash = 0;
	boolean Crashed = 0;
	
	byte team;
	unsigned long weapon;
	
/////////////////////////////////////////////////////////////////////////
	
	byte devCount;
	byte devFind[3];
	
	int devRSSI0;
	int devRSSI1;
	int devRSSI2;
		
	byte devName0[20];
	byte devName1[20];
	byte devName2[20];
		
	byte devAddress0[6];
	byte devAddress1[6];
	byte devAddress2[6];
	
	byte devAddressBuf[6];
	byte devAddressConnected[6];
	
/////////////////////////////////////////////////////////////////////////
	
	int roll;
	int pitch;
	int yaw;
	int throttle;
	
    int prevControl[4];
		
	int attitudeRoll;
	int attitudePitch;
	int attitudeYaw;
	
/////////////////////////////////////////////////////////////////////////
	
	int linkState;
	int rssi;
	byte battery;
		
	byte irMassageDirection;
  	unsigned long	irMassageReceive;
	
	byte droneState[7];	
	byte droneIrMassage[5];	
	
	s16 droneAttitude[3];
	byte droneGyroBias[6];
	byte droneTrimAll[10];		
	byte droneTrimFlight[8];
	byte droneTrimDrive[2];

	s16 droneImuRawAndAngle[9];

	byte dronePressure[16];	
	byte droneImageFlow[8];
	byte droneButton[1];
	byte droneBattery[16];
	byte droneMotor[4];
	byte droneTemperature[8];

	s16 accel[3];
	s16 gyroRaw[3];
	s16 gyroAngle[3];

	s32 imageFlowX;
	s32 imageFlowY;

/////////////////////////////////////////////////////////////////////////

		long PreviousMillis;
		
/////////////////////////////////////////////////////////////////////////
private:
	long PreviousBuzz;		

	long timeOutSendPreviousMillis;

	unsigned long HealthTime;

//	SoftwareSerial DEBUG_SERIAL = SoftwareSerial(8,9);
};

extern CoDroneClass CoDrone;

#endif 
