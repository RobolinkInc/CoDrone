/*********************************************************************************
  Control - Analog Joystick Control - Drone Battle
  드론을 조이스틱으로 조종합니다.
   - 스마트 보드를 조종기 형태로 조립해서 사용하는 예제입니다.
   - 밑면 센서 가장 끝 1번센서에 손을 대면 드론이 정지하게 됩니다.
   - 밑면 센서 가장 끝 8번센서에 손을 대면 무기를 발사하게 됩니다.
   - BattleBegin 에 자신이 원하는 팀을 선택합니다. - 4가지 팀(같은 팀끼리의 무기는 맞지 않습니다.)
   - 팀과 상관없이 많은 인원이 개인전을 하는 경우에는 FREE_PLAY를 선택합니다. 

   - 초기 에너지 수치는 8이며, 상대에게 무기를 맞을때마다 에너지가 줄어들게 됩니다.
   - 무기를 8회 맞아서 에너지가 0이되면, 게임에서 지게 됩니다.
   - 무기를 맞을 때마다 맞은 조종기에서 비프음과 현재 에너지 상태가 LED로 표시 됩니다.   
   - 에너지가 0이 되면, 드론의 날개쪽 LED가 꺼지게 되며 조종기에서 멜로디가 나옵니다.
***********************************************************************************/

#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);    // 가장 가까운 위치의 드론과 연결
  CoDrone.DroneModeChange(Flight);      // 드론을 플라이트 모드로 설정합니다. (비행형)
  CoDrone.BattleBegin(TEAM_BLUE);       //  팀 선택 : TEAM_RED  / TEAM_BLUE /  TEAM_GREEN  / TEAM_YELLOW / FREE_PLAY
}

void loop()
{
  CoDrone.BattleReceive();        // IR-Data 수신

  byte bt1 = digitalRead(11);       // ■ □ □ □ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt4 = digitalRead(14);       // □ □ □ ■ □ □ □   밑면 적외선 센서를 입력으로 사용
  byte bt8 = digitalRead(18);       // □ □ □ □ □ □ ■   밑면 적외선 센서를 입력으로 사용


  if (bt1 && !bt4 && !bt8)          // 밑면 센서 가장 끝 1번센서에 손을 대면 실행합니다.
  {
    CoDrone.FlightEvent(Stop);     // 드론을 긴급 정지시킵니다.
  }

  if (!bt1 && !bt4 && bt8)           // 밑면 센서 가장 끝 1번센서에 손을 대면 실행합니다.
  {
    CoDrone.BattleShooting();        // 무기를 발사합니다.
    CoDrone.ButtonPreesHoldWait(18); // 버튼을 뗄때까지 기다립니다. (연속적인 발사를 막습니다.)
  }

  if (!bt1 && bt4 && !bt8)            // 밑면 가운데 센서에 손을 대면 실행합니다.
  {
    CoDrone.FlightEvent(Landing);    // 드론을 서서히 착륙 시킵니다.
  }

  // ----------------------------------------------------------------------------------------------------------------------------//

  YAW = -1 * CoDrone.AnalogScaleChange(analogRead(A3));   // 아날로그 3번 핀의 값을 YAW 값으로 사용합니다.       - 좌우회전
  THROTTLE  = CoDrone.AnalogScaleChange(analogRead(A4));  // 아날로그 4번 핀의 값을 THROTTLE 값으로 사용합니다.  - 승하강
  ROLL = -1 * CoDrone.AnalogScaleChange(analogRead(A5));  // 아날로그 5번 핀의 값을 ROLL 값으로 사용합니다.      - 좌우이동
  PITCH = CoDrone.AnalogScaleChange(analogRead(A6));      // 아날로그 6번 핀의 값을 PITCH 값으로 사용합니다.     - 전후진
  CoDrone.Control(SEND_INTERVAL);                         // 제어 신호를 보냅니다. 통신이 안정하게 가도록 시간을 두고 보냄(최소 50ms)

}
