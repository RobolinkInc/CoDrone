#include "CoDrone.h"
#include "Arduino.h"
#include <EEPROM.h>

//-------------------------------------------------------------------------------------------------------//
//------------------------------------------- GetData ---------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::ReceiveGetData(byte _reqType)
{
	byte *_reqCheckType;

	sendCheckFlag = 1;
	Send_Command(cType_Request, _reqType);
	
//---------------------------------------------------------------------------------//
	
	if 		(_reqType == Req_Attitude) 			_reqCheckType = &receiveAttitudeSuccess;
	else if (_reqType == Req_Battery) 			_reqCheckType = &receiveBatterySuccess;
	else if (_reqType == Req_Range) 			_reqCheckType = &receiveRangeSuccess;
	else if (_reqType == Req_State) 			_reqCheckType = &receiveStateSuccess;
	else if (_reqType == Req_ImageFlow) 		_reqCheckType = &receiveOptSuccess;
	else if (_reqType == Req_Pressure) 			_reqCheckType = &receivePressureSuccess;
	else if (_reqType == Req_TrimFlight) 		_reqCheckType = &receiveTrimSuccess;
	else if (_reqType == Req_ImuRawAndAngle)	_reqCheckType = &receiveAccelSuccess;

//--------------------------------------------------------------------------------//
	*_reqCheckType = 0;

	long oldTime = millis();
	while(!*_reqCheckType)
	{
		Receive();
		if (oldTime + 500 < millis()) break; //time out check
	}
}
//--------------------------------------------------------------------------------//
/*
 *	function	:	getPressure()
 *	description : 	getPressure() is function for get pressure from barometer sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- pressure 	: int
 */
int CoDroneClass::getPressure()
{
	ReceiveGetData(Req_Pressure);
	return pressure;
}

/*
 *	function	:	getDroneTemp()
 *	description : 	getDroneTemp() is function for get temperature from barometer sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- temperature 	: int
 */
int CoDroneClass::getDroneTemp()
{
	ReceiveGetData(Req_Pressure);
	return temperature;
}

/*
 *	function	:	getState()
 *	description : 	getState() is function for get drone state.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Have to use with enum modeFlight
					- fMode_None = 0,
					- fMode_Ready,
					- fMode_TakeOff,
					- fMode_Flight, 
					- fMode_Flip, 	
					- fMode_Stop, 	
					- fMode_Landing,
					- fMode_Reverse,
					- fMode_Accident,
					- fMode_Error, 
					- fMode_EndOfType
 *	param 		:	none
 *	return 	 	:	
 *					- droneState[3] 	: int
 */

int CoDroneClass::getState()
{
	ReceiveGetData(Req_State);
	return droneState[2];
}
/*
 *	function	:	getHeight()
 *	description : 	getHeight() is function for get height from IR distance sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- height 	: int
 */
int CoDroneClass::getHeight()
{
	ReceiveGetData(Req_Range);
	return sensorRange[5];
}

/*
 *	function	:	getBatteryPercentage()
 *	description : 	getBatteryPercentage() is function for get battery percentage.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					If you request in a 5 seconds, it's return data that save in a header file.
 *	param 		:	none
 *	return 	 	:	
 *					- Battery_Percent 	: int
 */

int CoDroneClass::getBatteryPercentage()
{
	ReceiveGetData(Req_Battery);
	return batteryPercent;
}

/*
 *	function	:	getBatteryVoltage()
 *	description : 	getBatteryVoltage() is function for get battery voltage.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					If you request in a 5 seconds, it's return data that save in a header file.
 *	param 		:	none
 *	return 	 	:	
 *					- Battery_voltage 	: int
 */
int CoDroneClass::getBatteryVoltage()
{
	ReceiveGetData(Req_Battery);
	return batteryVoltage;
}

/*
 *	function	:	getAceelerometer()
 *	description : 	getAceelerometer() is function for get Aceelerometer value from gyro sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure acceldata  is made of x, y, and z
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: acceldata
 */

acceldata CoDroneClass::getAccelerometer()
{
	ReceiveGetData(Req_ImuRawAndAngle);

	acceldata result;
	result.x = ImuAccX;
	result.y = ImuAccY;
	result.z = ImuAccZ;
	return result;
}

/*
 *	function	:	getTrim()
 *	description : 	getTrim() is function for get current trim value for flight.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also.
 *					Structure trim data has roll, pitch, yaw, and throttle.
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: int
 */

trimdata CoDroneClass::getTrim()
{
	ReceiveGetData(Req_TrimFlight);

	trimdata result;
	result.roll = TrimAll_Roll;
	result.pitch = TrimAll_Pitch;
	result.yaw = TrimAll_Yaw;
	result.throttle = TrimAll_Throttle;

	return result;
}

/*
 *	function	:	getOptFlowPosition()
 *	description : 	getOptFlowPosition() is function for get x,y position value from optical flow sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure optdata  is made of x, and y
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: optdata
 */

optdata CoDroneClass::getOptFlowPosition()
{
	ReceiveGetData(Req_ImageFlow);

	optdata result;
	result.x = fVelocitySumX;
	result.y = fVelocitySumY;
	return result;
}

/*
 *	function	:	getAngulerSpeed()
 *	description : 	getAngulerSpeed() is function for get gyro value from gyro sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure gyrodata  is made of roll, pitch, and yaw
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: gyrodata
 */

gyrodata CoDroneClass::getAngularSpeed()
{
	ReceiveGetData(Req_ImuRawAndAngle);

	gyrodata result;
	result.roll = ImuGyroRoll;
	result.pitch = ImuGyroPitch;
	result.yaw = ImuGyroYaw;

	return result;
}

/*
 *	function	:	getGyroAngles()
 *	description : 	getGyroAngles() is function for get angle value.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure angledata  is made of roll, pitch, and yaw
 *	param 		:	none
 *	return 	 	:	
 *					- result	: angledata
 */

angledata CoDroneClass::getGyroAngles()
{
	ReceiveGetData(Req_Attitude);

	angledata result;
	result.roll = attitudeRoll;
	result.pitch = attitudePitch;
	result.yaw = attitudeYaw;

	return result;
}
/*
 *	function	:	isUpsideDown()
 *	description : 	isUpsideDown() is function for check drone is upside down or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isUpsideDown()
{
	if(getState() == fMode_Flip)
		return true;
	return false;
}


/*
 *	function	:	isFlying()
 *	description : 	isFlying() is function for check drone is flying or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isFlying()
{
	if(getState() == fMode_Flight)
		return true;
	return false;
}


/*
 *	function	:	isReadyToFly()
 *	description : 	isReadyToFly() is function for check drone is ready or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isReadyToFly()
{
	if(getState() == fMode_Ready)
		return true;
	return false;
}

/*
 *	function	:	lowBatteryCheck()
 *	description : 	lowBatteryCheck() is function for check lowBattery or not 
 *					If drone is 30% or low return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::lowBatteryCheck()
{
	if(getBatteryPercentage() <=30)
		return true;
	return false;
}

//-------------------------------------------------------------------------------------------------------//

