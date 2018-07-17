#include <CoDrone.h>

void setup(){

    CoDrone.begin(115200);
    CoDrone.AutoConnect(NearbyDrone);

    CoDrone.FlightEvent(TakeOff);
    delay(2000);

    CoDrone.Buzz(2000,7);
    THROTTLE = 70;
    CoDrone.Control();
    delay(2000);

    CoDrone.Buzz(4000, 7);
    PITCH = 60;
    CoDrone.Control();
    delay(2000);

    CoDrone.Buzz(6000,7);
    CoDrone.FlightEvent(Landing);
}

void loop(){
}