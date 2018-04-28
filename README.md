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
1.5.2
---------------------------------------------------------------------
- fix syntex takeOff() function to takeoff()

- add buzzer sound when try to pair and after pairing 

From 1.5.0
added Function:
-go(), move(), turn(), request----() etc..

- we keep old functions but add new functions for younger kid and user

- New library is more close to natural language

- Request battery info in new function only request once int 3 seconds 

- If you request more than once in 3 seconds, It will request at first time and use that data for 3 second


