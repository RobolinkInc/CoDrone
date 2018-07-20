/*------------------------------------------------------------------
// This is the sample code for Lesson 3D:ControllerLEDs
// https://basecamp.robolink.com/cwists/preview/753x
-------------------------------------------------------------------*/
#include <CoDrone.h>

void setup()
{
    CoDrone.begin(115200);
    CoDrone.pair();     // Pair with one the last drone this remote paired with

    CoDrone.takeoff();  // take off
} 
void loop()
{
    byte bt1 = digitalㅌRead(11);
    byte bt4 = digitalRead(14);
    byte bt8 = digitalRead(18);
    LEDs(0,0);
    //kill switch
    if (bt1) {
        CoDrone.emergencyStop();
    } 
    if (PAIRING == true) {
        YAW = -1 * CoDrone.AnalogScaleChange(analogRead(22));
        THROTTLE = CoDrone.AnalogScaleChange(analogRead(23));
        ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(24));
        PITCH = CoDrone.AnalogScaleChange(analogRead(25));
        CoDrone.move();
    } 
    while (bt8) {
        bt8 = digitalRead(18);
        LEDs(1,1);
        // if you move left joystick move leff while hit button 8 change LED to yellow dimming
        if (CoDrone.AnalogScaleChange(analogRead(22)) > 50) {
            CoDrone.setArmLED(255,255,0);
            CoDrone.setArmMode(LED_DIMMING);
        }
        // if you move left joystick move right while hit button 8 change LED to blue dimming
        else if (CoDrone.AnalogScaleChange(analogRead(22)) < -50 ) {
            CoDrone.setArmLED(0,0,255);
            CoDrone.setArmMode(LED_DIMMING);
        }
    }
} 
void LEDs(int p13, int p16) {
    digitalWrite(13, p13);
    digitalWrite(16, p16);
}







