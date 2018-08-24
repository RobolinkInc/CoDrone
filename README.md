# CoDrone
//A collection of CoDrone Libraries and Updates<br>
//Written in Arduino IDE<br>
//For the Robolink community <br>
//Learn more at www.Robolink.com<br>

//Collection includes:<br>
//CoDrone.h<br>
//CoDrone.cpp<br>
//CoDrone_led.cpp<br>
//CoDrone_request.cpp<br>
//CoDrone_send.cpp<br>
//CoDrone_util.cpp<br>
//EEPROM.h<br>
//EEPROM.cpp<br>
//And various examples for each library<br>


/*******************************************************************/
PATCH NOTES
/*******************************************************************/
2.1.0
---------------------------------------------------------------------
Update New LED function<br>
new function can change color and mode at the same time<br>
Change arm and eye color at the same time(setAll function) has no delay to change both<br>
Check Document for more detail : docs.robolink.com

syntax

	void setEyeLED(byte Color,byte Mode);
	void setEyeLED(byte Color, byte Mode, byte Interval);
	void setEyeLED(byte R, byte G, byte B, byte Mode);
	void setEyeLED(byte R, byte G, byte B, byte Mode, byte Interval);
	void setArmLED(byte Color, byte Mode);
	void setArmLED(byte Color, byte Mode, byte Interval);
	void setArmLED(byte R, byte G, byte B, byte Mode);
	void setArmLED(byte R, byte G, byte B, byte Mode, byte Interval);
	void setAllLED(byte Color, byte Mode);
	void setAllLED(byte Color, byte Mode, byte Interval);
	void setAllLED(byte R, byte G, byte B, byte Mode);
	void setAllLED(byte R, byte G, byte B, byte Mode, byte Interval);
	void setEyeDefaultLED(byte R, byte G, byte B, byte Mode);
	void setEyeDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval);
	void setArmDefaultLED(byte R, byte G, byte B, byte Mode);
	void setArmDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval);
	void setAllDefaultLED(byte R, byte G, byte B, byte Mode);
	void setAllDefaultLED(byte R, byte G, byte B, byte Mode, byte Interval);


