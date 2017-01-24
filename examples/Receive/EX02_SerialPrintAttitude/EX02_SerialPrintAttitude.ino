/**********************************************************************************************
  - Attitude To SerialMonitor
  드론의 현재 자세 정보 (ROLL, PITCH, YAW) 값을 시리얼 모니터 창에 표시합니다.
  시리얼 모니터창을 열고 '115200 보드레이트'로 설정합니다.
  USB를 PC와 연결한채로 링크보드를 제어보드를 연결합니다.
  그상태에서 드론을 켜서 페어링이 되면 드론이 보내오는 자세 정보가 시리얼 모니터창에 표시됩니다.
************************************************************************************************/

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
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active);  //link module mode change => Active
  delay(100);
  CoDrone.Request_DroneAttitude();

  long oldTime = millis();
  boolean attitudeSuccess = false;
  CoDrone.receiveAttitudeSuccess = 0;       //receiveAttitudeSuccess flag init

  while (attitudeSuccess == false)            //receiveAttitudeSuccess check
  {
    CoDrone.Receive();
    if (CoDrone.receiveAttitudeSuccess == 1)   attitudeSuccess = true;
    if (oldTime + 1000 < millis()) break;
  }

  //---------------------------------------------------------------------------------------------//
  if (attitudeSuccess ==  true)
  {
    CoDrone.Send_LinkModeBroadcast(LinkModeMute);       //link module mode change => Mute
    delay(300);

    Serial.println("");
    Serial.println("--------- Now attitude -----------");
    Serial.print("ROLL\t");
    Serial.println(AttitudeROLL);
    Serial.print("PITCH\t");
    Serial.println(AttitudePITCH);
    Serial.print("YAW\t");
    Serial.println(AttitudeYAW);
    delay(500);
  }
  //---------------------------------------------------------------------------------------------//
}




