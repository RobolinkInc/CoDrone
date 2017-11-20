/*************************************************************************************
- Serial Print Attitude
Smartboard has to be connected with computer to see serial monitor
request drone's Angle information and print in serial monitor
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

void AttitudeToSerialMonitor() 
{ 
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
  delay(100); 
  CoDrone.Request_DroneAttitude(); 
  
  long oldTime = millis(); 
  while (CoDrone.receiveAttitudeSuccess == false) //receiveAttitudeSuccess check 
  { 
    CoDrone.Receive(); 
    if (oldTime + 1000 < millis()) break; //time out check 
  } 
  
  //---------------------------------------------------------------------------------------------// 
  if (CoDrone.receiveAttitudeSuccess == true) 
  { 
    CoDrone.Send_LinkModeBroadcast(LinkModeMute); //link module mode change => Mute 
    delay(300); 
    
    Serial.println(""); 
    Serial.println("--------- Now attitude -----------"); 
    Serial.print("ROLL : "); 
    Serial.println(AttitudeROLL); 
    Serial.print("PITCH : "); 
    Serial.println(AttitudePITCH); 
    Serial.print("YAW : "); 
    Serial.println(AttitudeYAW); 
    delay(500); 
  } 
  //---------------------------------------------------------------------------------------------// 
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active); //link module mode change => Active 
}



