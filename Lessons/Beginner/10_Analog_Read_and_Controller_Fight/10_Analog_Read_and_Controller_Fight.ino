#include <CoDrone.h>

void setup() {
    CoDrone.begin(115200);
    CoDrone.AutoConnect(NearbyDrone);
}

void loop() {if (PAIRING == true){  
    YAW = -1 * CoDrone.AnalogScaleChange(analogRead(22));   
    THROTTLE = CoDrone.AnalogScaleChange(analogRead(23));   
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(24));   
    PITCH = CoDrone.AnalogScaleChange(analogRead(25));   
    CoDrone.Control();   
    }

if (digitalRead(11) && !digitalRead(14) && !digitalRead(18)){
    CoDrone.FlightEvent(Stop);
    }
}







