#include <CoDrone.h>

int A = 1000;
int B = 500;
int C = 5;
int D = 2;
int E = 10;
int F = 3;

void setup(){  
    CoDrone.begin(115200);  
    CoDrone.AutoConnect(NearbyDrone);  
    CoDrone.FlightEvent(TakeOff);
    delay(A);  
    THROTTLE = (C*C) * F;     //5 * 5 * 3  = 75  
    CoDrone.Control();  
    delay(A * D);             // 1000 * 2 = 2000  
    CoDrone.FlightEvent(Landing);
}

void loop(){
    byte bt8 = digitalRead(18);
    byte bt4 = digitalRead(14);
    byte bt1 = digitalRead(11);

    if(bt1 && bt4 && !bt8){
        CoDrone.FlightEvent(Stop);
    }
}







