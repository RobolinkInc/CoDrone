#include <CoDrone.h>

unsigned long StartTime;
int Step;

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
        CoDrone.FlightEvent(Stop);    
        Step = 0;  
    }  

    if (!bt1 && bt8) {    
        Step = Step + 1;    
        
        if (Step > 3) {      
            Step = 0;    
    }    
    CoDrone.Buzz(3000, 7);    
    delay(200);  
    }  

    if ( Step == 1) {    
       THROTTLE = 100;    
       CoDrone.Control(SEND_INTERVAL);  
    }  
    else if (Step == 2) {    
        PITCH = 70;    
        CoDrone.Control(SEND_INTERVAL);  
    }  
    else {    
        CoDrone.FlightEvent(Landing);  
    }
}