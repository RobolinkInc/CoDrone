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
  AttitudeToSerialMonitor(); 
  delay(500); 
} 

void RangeSensorToSerialMonitor() 
{ 
//---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(100); 
  CoDrone.Request_Range(); 
  
  long oldTime = millis(); 
  while (CoDrone.receiveRangeSuccess == false) //receiveRangeSuccess check 
  { 
    CoDrone.Receive(); 
    if (oldTime + 1000 < millis()) break; //time out check 
  } 
  //---------------------------------------------------------------------------------------------// 
  if (CoDrone.receiveRangeSuccess == true) 
  { 
  CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
  delay(300); 
  
  Serial.println(""); 
  Serial.println("--------------- Sensor ---------------"); 
  Serial.print("range : "); 
  Serial.println(CoDrone.sensorRange[5]);  
  Serial.println("-------------------------------------- "); 
  delay(500); 
  } 
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
}



