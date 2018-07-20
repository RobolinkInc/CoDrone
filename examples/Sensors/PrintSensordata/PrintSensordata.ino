/*------------------------------------------------------------------
PrintSensorData
Objective   : print sensor data after grab from request function
syntax      : 
description : BLE board has call Multiplaxer to talk with arduino and drone
              or arduino and pc. we need to change using Send_LinkModeBroadcast()
              Active mode will be talking with Drone and mute mode will be talking
              with PC. you can only print when you are mute mode
              you should check buadrate before you are doing 
-------------------------------------------------------------------*/
#include <CoDrone.h> 

void setup() 
{ 
  CoDrone.begin(115200); 
  CoDrone.pair();
  delay(500); 
} 

void loop() 
{ 
  int height;
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(50); 
  height = CoDrone.getHeight();
   
  CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
  delay(50); 
  
  Serial.println(""); 
  Serial.println("--------------- Sensor ---------------"); 
  Serial.print("height : "); 
  Serial.println(height);  
  Serial.println("-------------------------------------- "); 
  delay(50); 
   
  // CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  // delay(50); 
} 




