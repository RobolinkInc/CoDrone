/*------------------------------------------------------------------
// This is the sample code for Lesson 3D:ControllerLEDs
// https://basecamp.robolink.com/cwists/preview/810x
-------------------------------------------------------------------*/

void setup() {
    Serial.begin(9600);
} 
void loop() {
    Test();
    Serial.println("hi");
    delay(1000);
}
 
void Test() {
    static int i = 0;
    if ( i < 10) {
        Serial.print(i);
        i++;
    }
    else {
        i = 0;
        Serial.println();
    }
}







