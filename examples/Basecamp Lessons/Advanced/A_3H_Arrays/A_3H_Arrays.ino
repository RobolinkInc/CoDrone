/*------------------------------------------------------------------
// This is the sample code for Lesson 3D:ControllerLEDs
// https://basecamp.robolink.com/cwists/preview/828x
-------------------------------------------------------------------*/
#include<CoDrone.h>

int melody[] = {
  NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  1, 2, 3, 4, 5, 6, 7, 8
};

void setup() {
	for (int thisNote = 0; thisNote<=7; thisNote++){
	  CoDrone.Buzz(melody[thisNote], noteDurations[3]);
	}
	delay(500);

	for (int thisNote = 7; thisNote>=0; thisNote--){
	  CoDrone.Buzz(melody[thisNote], noteDurations[3]);
	}
	delay(500);

	CoDrone.Buzz(melody[0],noteDurations[2]);
	delay(50);
	CoDrone.Buzz(melody[0],noteDurations[2]);
	CoDrone.Buzz(melody[1],noteDurations[1]);
	CoDrone.Buzz(melody[0],noteDurations[1]);
	CoDrone.Buzz(melody[3],noteDurations[1]);
	CoDrone.Buzz(melody[2],noteDurations[0]);
}

void loop(){
	
}







