/*****************************************************************
  - LowBatteryCheck
  LowBatteryCheck(level)
  로우 배터리 측정 (배터리 레벨 기준값)
  * 배터리 값은 0 ~ 100
  * 기준 값보다 배터리가 적다면 부저를 울립니다.
*******************************************************************/

#include <CoDrone.h>                   // 코드론을 사용하기 위한 헤더파일 

byte level = 50;                        // 이곳에서 설정한 값이 배터리 기준 값이 됨

void setup()
{
  CoDrone.begin(115200);                  // 드론 플러그의 기능 개시

  CoDrone.AutoConnect(NeardbyDrone);      // 가장 가까운 위치의 드론과 연결
  
  CoDrone.LowBatteryCheck(level);         // 만약 배터리가 입력한 기준값 보다 작다면 부저를 울려서 알려줌
 
}

void loop()
{
}
