#include <CoDrone.h>
int throttle[50], pitch[50], yaw[50], roll[50];
int j;
 
void setup() {
    CoDrone.begin(115200);
    CoDrone.AutoConnect(NearbyDrone);
}
 
void loop() {

    if (digitalRead(18)) {
        while (digitalRead(18) && j <= 48) {
            throttle[j] = ReadAndScale(23);            yaw[j] = ReadAndScale(22, -1);
            roll[j] = ReadAndScale(24, -1);
            pitch[j] = ReadAndScale(25);
            CoDrone.Buzz(3000, 50);
            delay(100);
            j = j + 1;
        }
        CoDrone.Buzz(8000, 2);
        CoDrone.Buzz(5000, 4);
        for ( int i = 0; i <= j; i++) {
            SetMove(throttle[i], yaw[i], pitch[i], roll[i]);
            CoDrone.Buzz(9000, 50);
            delay(100);
            if (digitalRead(11)) {
                CoDrone.FlightEvent(Stop);
                break;
            }
        }
        CoDrone.FlightEvent(Landing);
        j = 0;
    } 
    if (digitalRead(11)) {
        CoDrone.FlightEvent(Stop);
    } 
    SetMove(ReadAndScale(23), ReadAndScale(22, -1), ReadAndScale(25), ReadAndScale(24,-1));
}







