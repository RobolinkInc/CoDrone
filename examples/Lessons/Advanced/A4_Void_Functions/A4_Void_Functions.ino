#include <CoDrone.h> 

int StartFlag;
unsigned long StartTime; 

void setup() {  
    CoDrone.begin(115200);  
    CoDrone.AutoConnect(NearbyDrone);

    CoDrone.FlightEvent(TakeOff);
    delay(2000);
} 

void loop() {  
    byte bt1 = digitalRead(11);  
    byte bt4 = digitalRead(14);  
    byte bt8 = digitalRead(18);   

    if (bt8) {    
        StartFlag = 1;    
        StartTime = millis();  
    }   

    if (StartFlag == 1) {    
        if (millis() - StartTime < 2000) {      
            Maneuver(70, 0, 0, 0);    
        }    
        else if (millis() - StartTime < 5000) {      
            Maneuver(0, 0, 70, 0);    
        }    
        else {      
            CoDrone.FlightEvent(Landing);       
            StartFlag = 0;    
        }  
    }   

    if ( bt1) {    
        CoDrone.FlightEvent(Stop);    
        StartFlag = 0;  
        }
    } 

    void Maneuver (int throttle, int roll, int pitch, int yaw) {  
        THROTTLE = throttle;  
        ROLL = roll;  
        PITCH = pitch;  
        YAW = yaw;  
        CoDrone.Control();
    }







