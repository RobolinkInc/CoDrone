#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);  // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair(Nearest);  // pair to the first drone found
  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode

  delay(1000);
}

void loop() {

  //The following code prints your drone's 6-index byte array address to the Serial Monitor
  CoDrone.Send_LinkModeBroadcast(LinkModeMute);
  delay(50);

  Serial.println();
  Serial.print('{');
  for(int i=0; i<6; i++) {
    Serial.print(CoDrone.devAddress0[i]);

    if( i != 5) {
      Serial.print(", ");
    }
  }
  Serial.print('}');
  Serial.println();
  delay(1000); 

}


