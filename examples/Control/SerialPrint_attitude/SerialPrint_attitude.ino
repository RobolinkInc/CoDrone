/*************************************************************************************
- Serial Print Attitude
드론플러그와 스마트 키트를 pc에 연결한 상태로 드론을 페어링하여
드론의 자세값(ROLL, PITCH, YAW)을 읽어 1초마다 시리얼 모니터로 출력해주는 예제입니다.
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
   delay(1000);
}

void AttitudeToSerialMonitor()
{
    //---------------------------------------------------------------------------------------------//
    CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active);  //link module mode change => Active
    
    CoDrone.Request_DroneAttitude();    
    while (CoDrone.receiveAttitudeSuccess == 0) //receiveAttitudeSuccess check
    {
      CoDrone.Receive();
    }
    CoDrone.receiveAttitudeSuccess = 0;   //receiveAttitudeSuccess flag init
    
    //---------------------------------------------------------------------------------------------//    
    CoDrone.Send_LinkModeBroadcast(LinkModeMute);       //link module mode change => Mute
    delay(10);

    Serial.println("");
    Serial.println("--------- Now attitude -----------");
    Serial.print("ROLL\t");
    Serial.println(AttitudeROLL);
    Serial.print("PITCH\t");
    Serial.println(AttitudePITCH);
    Serial.print("YAW\t");
    Serial.println(AttitudeYAW);     
    //---------------------------------------------------------------------------------------------//
}




