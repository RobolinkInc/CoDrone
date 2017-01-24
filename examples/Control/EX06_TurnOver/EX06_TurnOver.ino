/*****************************************************************
  - Turn Over
  드론이 뒤집혔을 경우 드론의 자세를 복구합니다.
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);              // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);   // 가장 가까운 위치의 드론과 연결
  CoDrone.DroneModeChange(Flight);    // 드론을 플라이트 모드로 설정합니다. (비행형)

  delay(300);                          // 대기 시간

  if (PAIRING == true)                 // 연결(페어링)이 성공한 경우에만 실행
  {
    CoDrone.FlightEvent(TurnOver);    // 뒤집기

    delay(4000);                       // 실행 시간

    CoDrone.FlightEvent(Stop);        //멈춤

  }
}

void loop()
{
}
