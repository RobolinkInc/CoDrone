/*------------------------------------------------------------------
This is the sample code for Lesson 4C: Roulette
https://basecamp.robolink.com/cwists/preview/2218x
------------------------------------------------------------------*/

#include <CoDrone.h>
int interval = 225;     // Change this for faster or slower roulette switches
int stride = 1000;      // Change this for longer or shorter stride lengths
#define YawEnabled 1    // Set to 1 to enable Yaw. Set to 0 to disable Yaw.

unsigned long timer;
int roulette = 10;      // Ignore these numbers. These are just here to prevent 
int FLAG = 2;           // the code from starting right after the CoDrone pairs.

void setup() {  
  CoDrone.begin(115200);  
  CoDrone.pair();

  CoDrone.takeoff();
}

void loop() {  
  byte bt1 = digitalRead(11);  
  byte bt8 = digitalRead(18);  

  if (bt1) {    
    if (FLAG == 2) {      
      FLAG = 0;      
      
      CoDrone.Buzz(3000, 20);      
      CoDrone.Buzz(5000, 20);      
      CoDrone.Buzz(7000, 20);      
      CoDrone.Buzz(9000, 20);      
            
      delay(500);    
    }    
        
    else {      
      CoDrone.emergencyStop();      
      FLAG = 3;      
      roulette = 10;    
    }  
  }  

  if (!bt1 && bt8) {    
    FLAG = 1;    
    CoDrone.Buzz(3000, 20);    
    CoDrone.Buzz(6000, 20);    
    CoDrone.Buzz(5000, 20);    
    CoDrone.Buzz(8000, 20);    
    delay(200);    
    timer = millis();  
  }  

  if ( millis() - timer > interval && FLAG == 0) {    
    timer = millis();    
    roulette = (int) random(0, 4);    
    
    switch (roulette) {      
      case 0:        
        digitalWrite(13, HIGH);        
        digitalWrite(16, HIGH);        
        break;      
      case 1:        
        digitalWrite(13, LOW);        
        digitalWrite(16, HIGH);        
        break;      
      case 2:        
        digitalWrite(13, LOW);        
        digitalWrite(16, LOW);        
        break;      
      case 3:        
        digitalWrite(13, HIGH);        
        digitalWrite(16, LOW);        
        break;    
    }    
    
    CoDrone.Buzz(2000 * (Roulette + 1), 7);  
  }  

  if (FLAG == 1) {    
    if ( millis() - timer < stride) {      
      switch (roulette) {        
        case 0:          
          CoDrone.setRoll(0);
          CoDrone.setPitch(50);
          CoDrone.setYaw(0);
          CoDrone.setThrottle(0);
          break;        
        case 1:          
          CoDrone.setRoll(50);
          CoDrone.setPitch(0);
          CoDrone.setYaw(0);
          CoDrone.setThrottle(0);      
          break;        
        case 2:
          CoDrone.setRoll(0);
          CoDrone.setPitch(-50);
          CoDrone.setYaw(0);
          CoDrone.setThrottle(0);     
          break;
        case 3:
          CoDrone.setRoll(-50);
          CoDrone.setPitch(50);
          CoDrone.setYaw(0);
          CoDrone.setThrottle(0);
          break;
      }      
      
      CoDrone.move();   
    }    

    else {      
      FLAG = 0;    
    }  
  }  
    
  if (FLAG == 0) {    
    if (YawEnabled == 1) {      
      CoDrone.setYaw(-1 * CoDrone.AnalogScaleChange(analogRead(A3)) / 2);
      CoDrone.setThrottle(0);
    }    
    
    CoDrone.setThrottle(CoDrone.AnalogScaleChange(analogRead(A4)));
    CoDrone.move();  
  }
}