/*****************************************************************
  - Circle
  드론의 전면이 중심을 바라보며 원을 한바퀴를 돌아 원점으로 돌아옵니다.
*******************************************************************/

#include <CoDrone.h>  // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);             // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);  // 가장 가까운 위치의 드론과 연결
  CoDrone.DroneModeChange(Flight);   // 드론을 플라이트 모드로 설정합니다. (비행형)

  delay(300);                          // 대기 시간

  if (PAIRING == true)
  {
    CoDrone.FlightEvent(TakeOff);     // 이륙

    delay(1000);                       // 대기 시간

    THROTTLE = 60;                     // THROTTLE 값 입력

    //90도 반원 회전 4번 반복
    for (int i = 0; i < 4; i++)
    {
      THROTTLE  = THROTTLE  - 10;      // THROTTLE 값 입력
      YAW = -80;                       // YAW 값 입력
      ROLL = 50;                       // ROLL 값 입력
      CoDrone.Control();              // 조종값 전송

      delay(1500);                     // 대기 시간
    }

    CoDrone.FlightEvent(Stop);        // 멈춤

  }
}

void loop()
{
}
