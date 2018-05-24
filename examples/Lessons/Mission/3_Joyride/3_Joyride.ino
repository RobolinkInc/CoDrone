#include <CoDrone.h>

void setup() {  
    CoDrone.begin(115200);  
    CoDrone.AutoConnect(NearbyDrone);  
    delay(2000);
}

void loop() {  
    byte bt1 = digitalRead(11);      
    byte bt2 = digitalRead(12);  
    byte bt4 = digitalRead(14);  
    byte bt7 = digitalRead(17);  
    byte bt8 = digitalRead(18);  

    if (bt1 && bt8 && !bt2 && !bt7) {    
        THROTTLE = 60;  
    }  
    else if (bt1 && !bt8 && bt2 && !bt7) {    
        ROLL = -30;  
    }  
    else if (!bt1 && bt8 && !bt2 && bt7) {    
        ROLL = 30;  
    }  
    else if (!bt1 && !bt8 && bt2 && bt7) {    
        THROTTLE = -50;  
    }  

    CoDrone.Control();  

    if (bt4) {    
        CoDrone.FlightEvent(Stop);  
    }
}