/*------------------------------------------------------------------
// This is the sample code for Lesson 3D:ControllerLEDs
// https://basecamp.robolink.com/cwists/preview/970x
-------------------------------------------------------------------*/
#include <CoDrone.h> 

int StartFlag;
unsigned long StartTime; 

void setup() {  
    CoDrone.begin(115200);  
    CoDrone.pair();     // Pair with one the last drone this remote paired with

    CoDrone.takeoff();
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
            myMoveFunc(70, 0, 0, 0);    
        }    
        else if (millis() - StartTime < 5000) {      
            myMoveFunc(0, 0, 70, 0);    
        }    
        else {      
            CoDrone.land();
            StartFlag = 0;    
        }  
    }   
    
    //kill switch
    if ( bt1) {    
        CoDrone.emergencyStop();
        StartFlag = 0;  
        }
    } 
    //function is set roll pitch yaw throttle and make drone move
    void myMoveFunc (int throttle, int roll, int pitch, int yaw) {  
        CoDrone.setThrottle(throttle);
        CoDrone.setRoll(roll);
        CoDrone.setPitch(pitch);
        CoDrone.setYaw(yaw);
        CoDrone.move();
    }







