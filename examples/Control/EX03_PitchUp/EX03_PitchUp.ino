/*****************************************************************
  Pitch Up
  피치값을 입력해서 드론을 앞뒤로 이동할 수 있습니다.
  전진(양수)  :  1 ~  100
  후진(음수)  : -1 ~ -100
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NearbyDrone);    // 가장 가까운 위치의 드론과 연결
  
  CoDrone.DroneModeChange(Flight);       // 드론을 플라이트 모드로 설정합니다. (비행형)
  
  if (PAIRING == true)                   // 연결(페어링)이 성공한 경우에만 실행
  {
    CoDrone.FlightEvent(TakeOff);        // 이륙

    delay(2000);                          // 대기 시간

    PITCH = 100;                           // PITCH 값 입력
    CoDrone.Control();                   // 조종값 전송

    delay(500);                           // 대기 시간

    CoDrone.FlightEvent(Landing);        // 서서히 착륙
  }
}

void loop()
{

}


