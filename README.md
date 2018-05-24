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
1.5.4
---------------------------------------------------------------------
Added function:

- pair(); If you open new box smart inventor board has no address save so pair function try to connect with drone which is the nearest recommand to use this function first time with only one drone want to connect. after that pair() will use saved address to connect.

- pair(Nearest) : If you want to connect with another drone use this to save other drone's address to smart inventor board
