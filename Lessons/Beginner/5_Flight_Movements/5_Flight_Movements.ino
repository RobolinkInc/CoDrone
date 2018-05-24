#include <CoDrone.h>

void setup() {
    CoDrone.begin(115200);
    CoDrone.AutoConnect(NearbyDrone);

    CoDrone.FlightEvent(TakeOff);          //Take off for 2 seconds
    delay(2000);

    THROTTLE = 50;                         //Throttle for 2 seconds
    CoDrone.Control();
    delay(2000);

    PITCH = 30;                            //Pitch and roll for 2 seconds
    ROLL = -30;
    CoDrone.Control();
    delay(2000);

    CoDrone.FlightEvent(Landing);          //Land

}

void loop () {
    byte bt8 = digitalRead(18);
    byte bt4 = digitalRead(14);
    byte bt1 = digitalRead(11);

    if(bt1 && bt4 && !bt8){
        CoDrone.FlightEvent(Stop);
    }
}







