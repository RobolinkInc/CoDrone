#include <CoDrone.h>

void setup()
{
    CoDrone.begin(115200);
    CoDrone.AutoConnect(NearbyDrone);

    CoDrone.FlightEvent(TakeOff);
    delay(2000);
} 
void loop()
{
    byte bt1 = digitalRead(11);
    byte bt4 = digitalRead(14);
    byte bt8 = digitalRead(18);
    LEDs(0,0);
    if (bt1) {
        CoDrone.FlightEvent(Stop);
    } 
    if (PAIRING == true) {
        YAW = -1 * CoDrone.AnalogScaleChange(analogRead(22));
        THROTTLE = CoDrone.AnalogScaleChange(analogRead(23));
        ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(24));
        PITCH = CoDrone.AnalogScaleChange(analogRead(25));
        CoDrone.Control(SEND_INTERVAL);
    } 
    while (bt8) {
        bt8 = digitalRead(18);
        LEDs(1,1);
        if (CoDrone.AnalogScaleChange(analogRead(22)) > 50) {
            CoDrone.LedColor( ArmDimming, Yellow, 7);
        }
        else if (CoDrone.AnalogScaleChange(analogRead(22)) < -50 ) {
            CoDrone.LedColor(ArmDimming, Blue, 7);
        }
    }
} 
void LEDs(int p13, int p16) {
    digitalWrite(13, p13);
    digitalWrite(16, p16);
}







