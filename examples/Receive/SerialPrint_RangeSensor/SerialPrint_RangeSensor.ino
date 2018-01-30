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
  angledata save;
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(100); 
  save = CoDrone.getAngles();
  //---------------------------------------------------------------------------------------------// 
   
  CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
  delay(300); 
  
  Serial.println(""); 
  Serial.println("--------------- Sensor ---------------"); 
  Serial.print("roll : "); 
  Serial.println(save.roll);
  Serial.print("pitch : "); 
  Serial.println(save.pitch);
  Serial.print("yaw : "); 
  Serial.println(save.yaw);  
  Serial.println("-------------------------------------- "); 
  delay(500); 
   
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
}



