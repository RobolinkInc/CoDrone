// This is the sample code for Lesson 2A: Upload Code
// https://basecamp.robolink.com/cwists/preview/685x

void setup() {

  pinMode(11, OUTPUT);
  pinMode(18, OUTPUT);

}
void loop() {

 digitalWrite(11, HIGH); 
 digitalWrite(18, LOW);
 delay(500); 
 digitalWrite(11, LOW);
 digitalWrite(18, HIGH);
 delay(500);
}







