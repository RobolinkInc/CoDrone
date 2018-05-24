#include <CoDrone.h>

unsigned long Timer;
int rand;

void setup() { 
 CoDrone.begin(115200);
 CoDrone.AutoConnect(NearbyDrone);

 CoDrone.FlightEvent(TakeOff);
 delay(2000);
}

void loop() { 
 if (millis() - Timer > 200) {
  rand = random(0, 4);  
  Indicator(rand);   
  Timer = millis();
  } 

 if ( digitalRead(18)) { 
   if (rand == 0) { 
     PITCH = 70; 
   }  
  else if (rand == 1) { 
     ROLL = 70; 
   } 
  else if (rand == 2) { 
     PITCH = -70; 
   }   
 else { 
     ROLL = - 70; 
   }  
  CoDrone.Control();
  CoDrone.Buzz(2000, 7); 
  CoDrone.Buzz(7000, 6);  
  delay(1000); 
 } 
 if (digitalRead(11)) { 
   CoDrone.FlightEvent(Stop); 
 }  

THROTTLE =  CoDrone.AnalogScaleChange(analogRead(23));
  CoDrone.Control();
}
void Indicator(int Direction) { 
 
if (Direction == 0) {  
  digitalWrite(13, HIGH);  
  digitalWrite(16, HIGH);  
  CoDrone.Buzz(2000, 4);
  }  
else if (Direction == 1) { 
 digitalWrite(13, LOW);   
 digitalWrite(16, HIGH);    
CoDrone.Buzz(4000, 4); 
 } 
 else if (Direction == 2) {  
  digitalWrite(13, LOW); 
   digitalWrite(16, LOW);  
  CoDrone.Buzz(6000, 4); 
 }  
else {   
 digitalWrite(13, HIGH);  
  digitalWrite(16, LOW);   
 CoDrone.Buzz(8000, 4); 
 }
}







