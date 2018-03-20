/*************************************************************************************
- Serial Print RangeSensor
Smartboard has to be connected with computer to see serial monitor
request drone's height information and print in serial monitor
***************************************************************************************/

#include <CoDrone.h> 

void setup() 
{ 
  CoDrone.begin(115200); 
  CoDrone.AutoConnect(NearbyDrone); 
  delay(500); 
} 

void loop() 
{ 
  RangeSensorToSerialMonitor(); 
  delay(50); 
} 

void RangeSensorToSerialMonitor() 
{ 
//---------------------------------------------------------------------------------------------// 
  int height;
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(50); 
  height = CoDrone.getHeight();
  //---------------------------------------------------------------------------------------------// 
   
  CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
  delay(50); 
  
  Serial.println(""); 
  Serial.println("--------------- Sensor ---------------"); 
  Serial.print("height : "); 
  Serial.println(height);  
  Serial.println("-------------------------------------- "); 
  delay(50); 
   
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
}



