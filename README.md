# CoDrone
//A collection of CoDrone Libraries and Updates
//Written in Arduino IDE
//For the Robolink community 
//Learn more at www.Robolink.com
//
//
//Collection includes:
//CoDrone.h
//CoDrone.cpp
//EEPROM.h
//EEPROM.cpp
//And various examples for each library


/*******************************************************************/
PATCH NOTES
/*******************************************************************/
1.4.5
---------------------------------------------------------------------
deleted Functions:
-PrintGyro(), PrintPressure(), PrintFlow(): delete print function

added Function:
-requestRangeSensor() : get range sensor data from IR rangesensor

other fix point:
-remove debug part for receive check(over 1000 lines) if you need it download lower version to use it

- Actual receive check data is range and attitude if you need more information for sensor data please let me know. We can get pressure, motor input voltage, raw gyro and accel data, bettery, etc. more.

- Add example for print range sensor and change default led color.

-time check merge one time check. Don't need other time check

-receive function is little change