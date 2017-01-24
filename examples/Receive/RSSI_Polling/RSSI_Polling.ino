/*****************************************************************
  RSSI Polling - RSSI 값 표시
  연결된 드론의 RSSI(신호세기) 값을 LED 불빛으로 표시합니다.    
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{

  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NearbyDrone);     // 가장 가까운 위치의 드론과 연결

  CoDrone.DisplayRSSI();                // 연결된 드론의 RSSI(신호세기) 값을 LED 불빛으로 표시
}

void loop()
{
 
}


