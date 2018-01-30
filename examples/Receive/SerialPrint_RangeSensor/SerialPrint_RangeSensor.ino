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
  delay(500); 
} 

void RangeSensorToSerialMonitor() 
{ 
//---------------------------------------------------------------------------------------------// 
  int height = 0;
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(100); 
  height = CoDrone.getHeight();
  //---------------------------------------------------------------------------------------------// 
  if (CoDrone.receiveRangeSuccess == true) 
  { 
  CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
  delay(300); 
  
  Serial.println(""); 
  Serial.println("--------------- Sensor ---------------"); 
  Serial.print("range : "); 
  Serial.println(height);  
  Serial.println("-------------------------------------- "); 
  delay(500); 
  } 
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
}



