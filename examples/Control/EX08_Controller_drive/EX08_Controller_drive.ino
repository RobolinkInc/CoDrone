/*****************************************************************
  Control - Analog Joystick Control
  드론을 조이스틱으로 조종합니다.
    스마트 보드를 조종기 형태로 조립해서 사용하는 예제입니다.
    * 드론에 바퀴를 장착한 형태일때 사용가능합니다.
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NearbyDrone);     // 가장 가까운 위치의 드론과 연결

  CoDrone.DroneModeChange(Drive);        // 드론을 드라이브 모드로 설정합니다. (바퀴 주행형)
}

void loop()
{
  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   밑면 적외선 센서를 입력으로 사용

  if (PAIRING == true)                // 연결(페어링)이 성공한 경우에만 실행
  {
    
    THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // 아날로그 4번 핀의 값을 THROTTLE 값으로 사용합니다.  - 전후 이동
    ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // 아날로그 5번 핀의 값을 ROLL 값으로 사용합니다.      - 좌우 이동
    
    CoDrone.Control(SEND_INTERVAL);                         // 제어 신호를 보냅니다. 통신이 안정하게 가도록 시간을 두고 보냄(최소 50ms)
  }
}


