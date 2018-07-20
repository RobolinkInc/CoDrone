/*------------------------------------------------------------------
Request
Objective   : How to request sensor data
syntax      : get"something"()
              *something can be : State, BatteryVoltage, BatteryPercentage,
                                  DroneTemp, Pressure, Height, Trim, GyroAngles,
                                  Accelerometer, OptFlowPosition, AnglearSpeed
description : BLE board has call Multiplaxer to talk with arduino and drone
              or arduino and pc. we need to change using Send_LinkModeBroadcast()
              Active mode will be talking with Drone and mute mode will be talking
              with PC. you can only print when you are mute mode
              you should check buadrate before you are doing 
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() { 
  int state;
  int voltage;
  int percentage;
  int temperature;
  int pressure;
  int height;
  acceldata accel;
  trimdata trim;
  optdata opt;
  gyrodata gyro;
  angledata angle;


  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair();
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  state = CoDrone.getState();
  voltage = CoDrone.getBatteryVoltage();
  percentage = CoDrone.getBatteryPercentage();
  temperature = CoDrone.getDronetemp();
  pressure = CoDrone.getPressure();
  height = getHeight();
  accel = getAccelerometer();
  trim = getTrim();
  opt = getOptFlowPosition();
  gyro = getAngularSpeed();
  angle = getGyroAngles();

}

void loop() {

}