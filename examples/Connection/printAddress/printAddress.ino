/*------------------------------------------------------------------
Print Address
Objective   : print drone address that connected now 
syntax      : 
description : This example are using "pair(Nearest)" to connect with
              the strongest signal drone and print out address with
              decimal number.
-------------------------------------------------------------------*/
#include <CoDrone.h> 

void setup() {  
  CoDrone.begin(115200);              // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair(Nearest);              // pair to the first drone found
  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode

  delay(5000);
}

void loop() {

  //The following code prints your drone's 6-index byte array address to the Serial Monitor
  //active serial moniter
  CoDrone.Send_LinkModeBroadcast(LinkModeMute);
  delay(50);

  Serial.println();
  Serial.print('{');
  //when it paired address will save in devAddressBuf array
  for(int i=0; i<6; i++) {
    Serial.print(CoDrone.devAddressBuf[i]);
    if( i != 5) {
      Serial.print(", ");
    }
  }
  Serial.print('}');
  Serial.println();
  delay(1000); 

}


