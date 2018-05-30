#include <CoDrone.h>
int Interval = 225;  // Change this for faster or slower Roulette switches
int Stride = 1000;   // Change this for longer or shorter stride lengths
#define YawEnabled 1 //Set to 1 to enable Yaw. Set to 0 to disable Yaw.

unsigned long Timer;
int Roulette = 10;   //Ignore these numbers. These are just here to prevent 
int FLAG = 2;        //the code from starting right after the CoDrone pairs.

void setup() {  
    CoDrone.begin(115200);  
    CoDrone.AutoConnect(NearbyDrone);

    CoDrone.FlightEvent(TakeOff);
    delay(2000);
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
            CoDrone.FlightEvent(Stop);      
            FLAG = 3;      
            Roulette = 10;    
        }  
    }  

    if (!bt1 && bt8) {    
        FLAG = 1;    
        CoDrone.Buzz(3000, 20);    
        CoDrone.Buzz(6000, 20);    
        CoDrone.Buzz(5000, 20);    
        CoDrone.Buzz(8000, 20);    
        delay(200);    
        Timer = millis();  
    }  

    if ( millis() - Timer > Interval && FLAG == 0) {    
        Timer = millis();    
        Roulette = Roulette + 1;    
        if (Roulette >= 4) {      
            Roulette = 0;    
        }    
        CoDrone.Buzz(2000 * (Roulette + 1), 7);  
    }  

    if (FLAG == 1) {    
        if ( millis() - Timer < Stride) {      
            switch (Roulette) {        
                case 0:          
                    PITCH = 50;          
                    break;        
                case 1:          
                    ROLL = 50;          
                    break;        
                case 2:          
                    PITCH = -50;          
                    break;        
                case 3:          
                    ROLL = -50;          
                    break;      
            }      
            CoDrone.Control(50);    
        }    
        else {      
            FLAG = 0;    
        }  
    }  
    
    if (FLAG == 0) {    
        if (YawEnabled == 1) {      
            YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3)) / 2;    
        }    
        THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));    
        CoDrone.Control(50);  
    }
}