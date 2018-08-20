#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>

//-------------------------------------------------------------------------------------------------------//
//------------------------------------------ Command ----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

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
}

void CoDroneClass::Send_Processing(byte _data[], byte _length, byte _crc[])
{
	delay(50);			
	sendingData = true;		//data sending start
	
	byte _packet[30];
	
	//START CODE  
	_packet[0] = START1;
	_packet[1] = START2;

	//HEADER & DATA
	for(int i = 0; i < _length + 3 ; i++)	 _packet[i+2] = _data[i];	   
	
	//CRC  
	_packet[_length + 4] =_crc[1];
	_packet[_length + 5] =_crc[0]; 
	
	
	if(sendDataControl == true)	DRONE_SERIAL.write(_packet, _length + 6);			//data send control
		
	#if defined(DEBUG_MODE_ENABLE)
	DEBUG_SERIAL.print("> SEND : ");	
	for(int i = 0; i < _length+6 ; i++)
	{
		DEBUG_SERIAL.print(_packet[i],HEX);	  	
		DEBUG_SERIAL.print(" ");	     
	}
	DEBUG_SERIAL.println("");	
	#endif 	
	
	
	sendingData = false;		//data sending end
}

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
//-------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------//
//--------------------------------------------- Send ----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::Send_Coordinate(byte mode)
{
	if(mode == cSet_Absolute)				Send_Command(cType_Coordinate, cSet_Absolute);
	else if(mode == cSet_Relative)	Send_Command(cType_Coordinate, cSet_Relative);
}
void CoDroneClass::Send_ClearGyroBiasAndTrim()
{
	sendCheckFlag = 1;
	Send_Command(cType_ClearGyroBiasAndTrim, 0);
}
void CoDroneClass::Send_ResetHeading()
{
	Send_Command(cType_ResetHeading, 0);
}
void CoDroneClass::disconnect()
{
	Send_Command(cType_LinkDisconnect, 0);
}

//-------------------------------------------------------------------------------------------------------//




//-------------------------------------------------------------------------------------------------------//
//----------------------------------------- Drone Event -------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::DroneModeChange(byte event)
{
	sendCheckFlag = 1;
	Send_Command(cType_ModeDrone, event);
	delay(300);
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
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
//----------------------------------------- Flight Evnet -------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
/*
 *	function	:	takeOff()
 *	description : 	takeOff() make the drone take off when drone is ready.
 *					If drone is upside down, it will do turtle turn instead of take off.
 *	param 		:	none
 *	return 	 	:	none
 */

void CoDroneClass::takeoff()
{
	CoDrone.Buzz(440,16);
	sendCheckFlag = 1;
	Send_Command(cType_FlightEvent, TakeOff);
	delay(3000);
	CoDrone.Buzz(440,16);
}

/*
 *	function	:	land()
 *	description : 	land() make the drone landing.
 *					Drone will going down and when drone is get close to floor, it will shut down motors.
 *	param 		:	none
 *	return 	 	:	none
 */

void CoDroneClass::land()
{
	sendCheckFlag = 1;
	Send_Command(cType_FlightEvent, Landing);
	for(int i=0; i<400; i=i+15)
  		CoDrone.Buzz(700-i,20);
	setRoll(0);
	setPitch(0);
	setYaw(0);
	setThrottle(0);
}


/*
 *	function	:	emargencyStop()
 *	description : 	emargencyStop() shut down the power of motors imediately.
 *					Use this function for kill switch.
 *	param 		:	none
 *	return 	 	:	none
 */

void CoDroneClass::emergencyStop()
{
	CoDrone.FlightEvent(Stop);
	for(int i=0;i<3;i++){
		CoDrone.Buzz(440,16);
		CoDrone.Buzz(330,16);
		CoDrone.Buzz(220,16);
 	}
	setRoll(0);
	setPitch(0);
	setYaw(0);
	setThrottle(0);
}

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Trim -----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::Set_Trim(byte event)
{
	sendCheckFlag = 1;
	Send_Command(cType_Trim, event);
}

void CoDroneClass::Set_TrimReset()
{
	Set_TrimFlight(0, 0, 0, 0);
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

	unsigned short crcCal = CRC16_Make(_packet, _len + 2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_len,_crc);
}


//-------------------------------------------------------------------------------------------------------//
//------------------------------------------- Control ---------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
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
	unsigned short crcCal = CRC16_Make(_packet, _len+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;
	Send_Processing(_packet,_len,_crc);
}

/*
 *	function	: 	move()
 *	description : 	move() function is using global variables to control drone .
 *					Drone will move with roll, pitch, yaw, throttle value infinitely.
 *				 	If you wanna set variable's value, use setter function.
 *				 	if you wanna check variable's value, use getter function.
 *
 *	param 		:	none
 *	return  	:	none
 */

void CoDroneClass::move(){
	// delay(50);
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

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	//send control message to drone(write serial)
	Send_Processing(_packet,_packet[1],_crc); 

}


/*
 *	function	: 	move(duration)
 *	description : 	move(duration) is extend function of move() for limit moving time.
 					Also instead of send command once, function will send every 100ms during duration time.
 *					Parameter "duration" will be set the timer for control drone.
 *
 *
 *	param 		:	
 					- duration	: float
 *	return 	 	:	none
 */
void CoDroneClass::move(float duration)
{
	//set the timer with duration.
	// duration have to convert second to millisecond
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move();
		//gap for command sending
		delay(50);
	}
	hover(1);
}


/*
 *	function	: 	move(roll, pitch, yaw, throttle)
 *	description : 	move(roll, pitch, yaw, throttle) is extend function of move() for set instance values for control.
 *					Parameter "roll, pitch, yaw, throttle " are one time value for control.
 *
 *	param 		:	
 * 					- roll		: int
 * 					- pitch		: int
 * 					- yaw		: int
 * 					- throttle	: int
 *	return 	 	:	none
 */
void CoDroneClass::move(int _roll, int _pitch, int _yaw, int _throttle)
{
	delay(50);
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
	_packet[2] = _roll;
	_packet[3] = _pitch;
	_packet[4] = _yaw;
	_packet[5] = _throttle;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;

	Send_Processing(_packet,_packet[1],_crc); 

}

/*
 *	function	: 	move(duration, roll, pitch, yaw, throttle)
 *	description : 	move(duration, roll, pitch, yaw, throttle) is extend function of move() for set instance values for control.
 *					Parameter "duration" will be set the timer for control drone.
 *					Parameter "roll, pitch, yaw, throttle " are one time value for control.
 *
 *	param 		:	
 *					- duration 	: float
 * 					- roll		: int
 * 					- pitch		: int
 * 					- yaw		: int
 * 					- throttle	: int
 *	return 	 	:	none
 */
void CoDroneClass::move(float duration, int _roll, int _pitch, int _yaw, int _throttle)
{
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(_roll, _pitch, _yaw, _throttle);
		delay(50);
	}
	hover(1);	
}


/*
 *	function	: 	go(direction)
 *	description : 	go(direction) is simple version of move function.
 *					Instead of using "roll, pitch, yaw throttle", using direction for control drone.
 *					Parameter "direction" is enum for set direction.
 *					Power will be set 50% defualt, It will move infinitely.
 *
 *	param 		:	
 *					- direction : int
 *	return 	 	:	none
 */
void CoDroneClass::go(int direction)
{
	int r,p,t;
	r = p = t = 0;
	
	switch(direction)
	{
		case direction_forward:
			p = 50;
			break;
		case direction_up:
			t = 50;
			break;
		case direction_right:
			r = 50;
			break;
		case direction_backward:
			p = -50;
			break;
		case direction_down:
			t = -50;
			break;
		case direction_left:
			r = -50;
			break;
	}
	move(r,p,0,t);
	unsigned long startMillis = millis();
	while(millis() - startMillis < 1000)
	{
		move(r,p,0,t);
		delay(50);
	}
	hover(1);

}

/*
 *	function	: 	go(direction, duration)
 *	description : 	go(direction, duration) adding duration parameter to go(direction)
 *					Parameter "direction" is enum for set direction.
 *					Parameter "duration" will be set the timer for control drone.
 *					Power will be set 50% defualt.
 *
 *	param 		:	
 *					- direction : int
 *					- duration 	: float
 *	return 	 	:	none
 */
void CoDroneClass::go(int direction, float duration)
{
	int r,p,t;
	r = p = t = 0;
	
	switch(direction)
	{
		case direction_forward:
			p = 50;
			break;
		case direction_up:
			t = 50;
			break;
		case direction_right:
			r = 50;
			break;
		case direction_backward:
			p = -50;
			break;
		case direction_down:
			t = -50;
			break;
		case direction_left:
			r = -50;
			break;
	}
	move(r,p,0,t);
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(r,p,0,t);
		delay(50);
	}
	if(duration != 0)
		hover(1);
}


/*
 *	function	: 	go(direction, duration, power)
 *	description : 	go(direction, duration, power) adding duration and power parameter to go(direction)
 *					Parameter "direction" is enum for set direction.
 *					Parameter "duration" will be set the timer for control drone.
 *					Parameter "power" will be set the movement power. (0 ~ 100)
 *
 *	param 		:	
 *					- direction : int
 *					- duration 	: float
 *					- power 	: int
 *	return 	 	:	none
 */
void CoDroneClass::go(int direction, float duration, int power)
{
	int r,p,t;
	r = p = t = 0;
	switch(direction)
	{
		case direction_forward:
			p = power;
			break;
		case direction_up:
			t = power;
			break;
		case direction_right:
			r = power;
			break;
		case direction_backward:
			p = (-1)*power;
			break;
		case direction_down:
			t = (-1)*power;
			break;
		case direction_left:
			r = (-1)*power;
			break;
	}
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(r,p,0,t);
		delay(50);
	}
	if(duration != 0)
		hover(1);
}


/*
 *	function	:  	turn(direction)
 *	description : 	turn(direction) is function for turn left or right infinitely
 *					Parameter "direction" is enum for set direction.
 *
 *	param 		:	
 *					- direction : int
 *	return 	 	:	none
 */
void CoDroneClass::turn(int direction)
{
	int y = 0;
	if (direction == direction_right)
		y = 50;
	else if (direction_left)
		y = -50;
	move(0,0,y,0);
	unsigned long startMillis = millis();
	while(millis() - startMillis < 1000)
	{
		move(0,0,y,0);
		delay(100);
	}
	hover(1);

}

/*
 *	function	: 	turn(direction, duration)
 *	description : 	turn(direction, duration) adding duration to for turn(direction)
 *					Parameter "direction" is enum for set direction.
 *					Parameter "duration" will be set the timer for control drone.
 *
 *	param 		:	
 *					- direction : int
 *					- duration 	: float
 *	return 	 	:	none
 */
void CoDroneClass::turn(int direction, float duration)
{
	int y = 0;
	if (direction == direction_right)
		y = 50;
	else if (direction_left)
		y = -50;
	move(0,0,y,0);
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(0,0,y,0);
		delay(50);
	}
	if(duration != 0)
		hover(1);

}


/*
 *	function	: 	turn(direction, duration, power)
 *	description : 	turn(direction, duration, power) adding duration and power parameter to turn(direction)
 *					Parameter "direction" is enum for set direction.
 *					Parameter "duration" will be set the timer for control drone.
 *					Parameter "power" will be set the movement power. (0 ~ 100)
 *
 *	param 		:	
 *					- direction : int
 *					- duration 	: float
 *					- power 	: int
 *	return 	 	:	none
 */
void CoDroneClass::turn(int direction, float duration, int power)
{
	int y = power ;
	if(direction == direction_left)
		y *= -1;
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(0,0,y,0);
		delay(50);
	}
	if(duration != 0)
		hover(1);
}




/*
 *	function	: 	hover(duration)
 *	description : 	hover(duration) is make drone hover when drone get this message for duration.
 *					Parameter "duration" will be set the timer for control drone.
 *
 *	param 		:	
 *					- duration 	: float
 *	return 	 	:	none
 */
void CoDroneClass::hover(float duration)
{
	move(0,0,0,0);
	delay(50);
	unsigned long startMillis = millis();
	while(millis() - startMillis < duration*1000)
	{
		move(0,0,0,0);
		delay(50);
	}
}
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Move -----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::goToHeight(int _range)
{
	int _dir = 0;
	//---------------------------------------------------------------//
	int value = getHeight();
	if (value < _range)			_dir = 1;	// up
	else if(value > _range)	_dir = -1;	// down
	//---------------------------------------------------------------//
	while(1)
	{
		value = getHeight();
		if (receiveRangeSuccess)
		{
			if((_dir > 0) && (value < _range))
			{
				THROTTLE = 30;
				CoDrone.Control();
			}	
			else if((_dir < 0) && (value > _range))
			{
				THROTTLE = -30;
				CoDrone.Control();
			}
			
			else
			{
				THROTTLE = 0;
				CoDrone.Control();
				break;
			}
		}
	}
}

void CoDroneClass::turnDegree(int _dir, int _angle)
{
	_dir = (_dir == RIGHT ? 1 : -1);
	angledata angle;
	angle = CoDrone.getGyroAngles();
	int yawPast = angle.yaw;

	_angle = _dir * (abs(_angle) - 5) + yawPast; //5 is bias

	while (1)
	{
		angle = CoDrone.getGyroAngles();
		int _nowAngle = angle.yaw;

		if (abs(yawPast - _nowAngle) > 180)	_angle -= _dir * 360;

		yawPast = _nowAngle;
		YAW = _dir * 20; //20 is power for yawing 

		if ((_dir > 0) && (_angle > _nowAngle)) 			CoDrone.Control();	// right turn
		else if ((_dir < 0) && (_angle < _nowAngle)) 	CoDrone.Control();	// left turn
		else
		{
			YAW = 0;
			CoDrone.Control();
			break;
		}
	}
}


//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::calibrate()
{
	byte _packet[9];
	byte _crc[2];

  	//header
	_packet[0] = dType_Command;
	_packet[1] = 0x02;

 	//data
	_packet[2] = 0x53;
	_packet[3] = 0;

	unsigned short crcCal = CRC16_Make(_packet, _packet[1]+2);
	_crc[0] = (crcCal >> 8) & 0xff;
	_crc[1] = crcCal & 0xff;


	Send_Processing(_packet,_packet[1],_crc);
	Send_Check(_packet,_packet[1],_crc);
	delay(50);
	CoDrone.Buzz(523,2);
	CoDrone.Buzz(659,2);
	CoDrone.Buzz(783,2);
	CoDrone.Buzz(1055,2);
}

/*
 *	function	:	flySequence(shape)
 *	description : 	flySequence(shape) is do some of squence fly
 *					Option is ZIPZAG, SWAY, SQUARE, CIRCLE, SPIRAL, TRIANGLE, HOP
 *	param 		:	
 *					- shape : int
 *	return 	 	:	none
 */
void CoDroneClass::flySequence(int shape)
{
	switch(shape)
	{
		case ZIGZAG :
			zigzag();
			break;

		case SWAY 	:
			sway();
			break;

		case SQUARE :
			square();
			break;

		case CIRCLE :
			circle();
			break;

		case SPIRAL :
			spiral();
			break;

		case TRIANGLE:
			triangle();
			break;

		case HOP:
			hop();
			break;
	}
}
/*
 *	function	:	sway()
 *	description : 	sway() is function for move drone left and right twice
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::sway()
{
	move(1, 50, 0, 0, 0);
  	move(1, -50, 0, 0, 0);
  	move(1, 50, 0, 0, 0);
  	move(1, -50, 0, 0, 0);
}


/*
 *	function	:	zigzag()
 *	description : 	zigzag() is function for move drone zigzag twice
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::zigzag()
{
	move(1, 50, 50, 0, 0);
  	move(1, -50, 50, 0, 0);
  	move(1, 50, 50, 0, 0);
  	move(1, -50, 50, 0, 0);
}


/*
 *	function	:	square()
 *	description : 	square() is function for fly square shape
 					It will fly right -> forward -> left -> back
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::square()
{
	move(2, 30, 0, 0, 0);
  	move(2, 0, 30, 0, 0);
  	move(2, -30, 0, 0, 0);
  	move(2, 0, -30, 0, 0);

}


/*
 *	function	:	triangle()
 *	description : 	triangle() is function for fly triangle shape
 					It will fly forward -> turn left 120 degree
 *					-> fly forward -> turn left 120 degree -> fly forward
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::triangle()
{
	turnDegree(RIGHT,30);
	move(2, 0,30,0,0);
  	turnDegree(LEFT,120);
  	move(2, 0,30,0,0);
  	turnDegree(LEFT,120);
  	move(2, 0,30,0,0);
}



/*
 *	function	:	hop()
 *	description : 	hop() is function for drone looks like hopping.
 *					Drone will go up and down for 1second each while flying forward
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::hop()
{
	move(30, 0, 50, 1);
	delay(1000);
    move(30, 0, -50, 1);
    delay(1000);
    hover(1);
}


/*
 *	function	:	circle()
 *	description : 	circle() is function for fly circle shape
 *					Drone will do yaw and roll at the same time
 *					yaw direction and roll direction is oposite
 *					this function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::circle()
{
	CoDrone.move(40, 0, 0, 0);
	delay(200);
	CoDrone.move(40, 0, -60, 0);
	delay(2500);
	CoDrone.move(40, 0, -50, 0);
	delay(1000);
	CoDrone.move(30, 0, 0, 0);
	delay(100);
	CoDrone.hover(1);
}


/*
 *	function	:	spiral()
 *	description : 	spiral() is function for fly spiral shape
 *					set yaw for -50 and increase roll with for loop
 *					after finish
 *					the function is called by flySequence()
 *	param 		:	none
 *	return 	 	:	none
 */
void CoDroneClass::spiral()
{
	for(int i = 0 ;i<5 ;i++){
    	setRoll(10+i*2);
    	setYaw(-50);
    	move();
    	delay(1000);
  	}
  hover(1);
  setRoll(0);
  setYaw(0);
}

//-------------------------------------------------------------------------------------------------------//
//------------------------------------------ Battle -----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::BattleBegin(byte teamSelect)
{
	team = teamSelect;
	byte _color = 0;
	
	switch (team) 
	{
		case TEAM_RED:
		  weapon = RED_MISSILE;
		  _color = Red;
		  break;	      
		case TEAM_BLUE:
			weapon = BLUE_MISSILE;
			_color = Blue;
		  break;	      
		case TEAM_GREEN:
			weapon = GREEN_MISSILE;
			_color = Green;
		  break;	      
		case TEAM_YELLOW:
			weapon = YELLOW_MISSILE;
			_color = Yellow;
		  break;	      
		case FREE_PLAY:
			weapon = FREE_MISSILE;
			_color = White;
		  break;
	}	

	CoDrone.LedColor(ArmHold, _color, 7);
	delay(300);
	CoDrone.LedColor(EyeDimming, _color, 7);
	delay (300);
}


void CoDroneClass::BattleReceive()
{
	Receive();
	if(irMessageReceive > 0)
	{
		if(team == TEAM_RED && irMessageReceive == BLUE_MISSILE || irMessageReceive == GREEN_MISSILE || irMessageReceive == YELLOW_MISSILE || irMessageReceive == FREE_MISSILE)	BattleDamageProcess();
		else if(team == TEAM_BLUE && irMessageReceive == RED_MISSILE || irMessageReceive == GREEN_MISSILE || irMessageReceive == YELLOW_MISSILE || irMessageReceive == FREE_MISSILE)	BattleDamageProcess();
		else if(team == TEAM_GREEN && irMessageReceive == BLUE_MISSILE || irMessageReceive == RED_MISSILE || irMessageReceive == YELLOW_MISSILE || irMessageReceive == FREE_MISSILE)	BattleDamageProcess();
		else if(team == TEAM_YELLOW &&  irMessageReceive == BLUE_MISSILE || irMessageReceive == GREEN_MISSILE || irMessageReceive == RED_MISSILE || irMessageReceive == FREE_MISSILE)	BattleDamageProcess();
		else if(team == FREE_PLAY && irMessageReceive == RED_MISSILE || irMessageReceive == BLUE_MISSILE || irMessageReceive == GREEN_MISSILE || irMessageReceive == YELLOW_MISSILE || irMessageReceive == FREE_MISSILE)	BattleDamageProcess();
		irMessageReceive = 0;
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

			for(int i = 0; i >= 3; i++)
			{
				CoDrone.Buzz(3000, 4);
				delay(100);
				CoDrone.Buzz(2000, 4);
				delay(100);
			}
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


//-------------------------------------------------------------------------------------------------------//

