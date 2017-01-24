/*****************************************************************
  Control - Analog Joystick Control
  드론을 조이스틱으로 조종합니다.
    스마트 보드를 조종기 형태로 조립해서 사용하는 예제입니다.
    밑면 센서 가장 끝 1번센서에 손을 대면 드론이 정지하게 됩니다.
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{  
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NeardbyDrone);     // 가장 가까운 위치의 드론과 연결

  CoDrone.DroneModeChange(Flight);       // 드론을 플라이트 모드로 설정합니다. (비행형)
}

void loop()
{
  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   밑면 적외선 센서를 입력으로 사용

  if (bt1 && !bt4 && !bt8)           // 밑면 센서 가장 끝 1번센서에 손을 대면 실행합니다.
  {
    CoDrone.FlightEvent(Stop);      // 드론을 정지시킵니다.
    // CoDrone.FlightEvent(Landing);      // 드론을 정지시킵니다.
  }

  if (!bt1 && !bt4 && bt8)           // 밑면 센서 가장 끝 1번센서에 손을 대면 실행합니다.
  {
//    CoDrone.FlightEvent(Stop);      // 드론을 정지시킵니다.
     CoDrone.FlightEvent(Landing);      // 드론을 정지시킵니다.
  }

  if (PAIRING == true)                // 연결(페어링)이 성공한 경우에만 실행
  {
    YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // 아날로그 3번 핀의 값을 YAW 값으로 사용합니다.       - 좌우회전
    THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // 아날로그 4번 핀의 값을 THROTTLE 값으로 사용합니다.  - 승하강
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // 아날로그 5번 핀의 값을 ROLL 값으로 사용합니다.      - 좌우이동
    PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // 아날로그 6번 핀의 값을 PITCH 값으로 사용합니다.     - 전후진
    CoDrone.Control(SEND_INTERVAL);                         // 제어 신호를 보냅니다. 통신이 안정하게 가도록 시간을 두고 보냄(최소 50ms)
  }
}


