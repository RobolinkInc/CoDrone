/*****************************************************************
  - Hudle Jump
  드론이 이륙과 전진을 하여 장애물을 넘어 착륙합니다.
*******************************************************************/

#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일

void setup() 
{
  CoDrone.begin(115200);              // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);   // 가장 가까운 위치의 드론과 연결
  CoDrone.DroneModeChange(Flight);    // 드론을 플라이트 모드로 설정합니다. (비행형)

  delay(400);                           // 대기 시간

  if (PAIRING == true)                  // 연결(페어링)이 성공한 경우에만 실행
  {
    CoDrone.FlightEvent(TakeOff);    // 이륙

    delay(1000);                      // 대기 시간
    //상승
    THROTTLE = 20;                    // THROTTLE 값 입력
    CoDrone.Control();               // 조종값 전송

    delay(500);                        // 대기 시간
    //전진
    THROTTLE = 0;                      // THROTTLE 전송
    PITCH = 100;                       // PITCH 전송
    ROLL = -10;                        // ROLL 전송
    CoDrone.Control();                // 조종값 전송

    delay(300);                        // 대기 시간
    //제자리 비행
    THROTTLE = 0;                     // THROTTLE 전송
    PITCH = 0;                        // PITCH 전송
    ROLL = 0;
    CoDrone.Control();

    delay(500);                       // 대기 시간

    CoDrone.FlightEvent(Landing);    // 착륙
  }
}

void loop() {
}
