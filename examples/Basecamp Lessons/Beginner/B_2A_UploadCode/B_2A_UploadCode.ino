/*------------------------------------------------------------------
This is the sample code for Lesson 2A: Upload Code
https://basecamp.robolink.com/cwists/preview/685x
-------------------------------------------------------------------*/

void setup() {

  pinMode(11, OUTPUT);					// Set pin 11 for output
  pinMode(18, OUTPUT);

}
void loop() {

  digitalWrite(11, HIGH);               // Turning pin 11 on
  digitalWrite(18, LOW);                // Turning pin 18 off
  delay(500);                           // Wait half a second
  digitalWrite(11, LOW);				// Now turn pin 11 off
  digitalWrite(18, HIGH);				// Now turn pin 18 on
  delay(500);
  
}