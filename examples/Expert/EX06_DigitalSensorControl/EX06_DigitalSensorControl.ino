/*****************************************************************
  - DigitalSensorControl
  7개의 디지털센서로 드론을 조종  
*******************************************************************/
#include <CoDrone.h> 

int slowUp = 80;        // 천천히 올라가는 정도의 호버링값
int slowDown = -100;    // 천천히 내려가는 정도의 호버링값
int slowTime = 10;      // 천천히 움직이는 속도 조절 : 커지면 천천히 올라감.

void setup()
{
  CoDrone.begin(115200);               // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);    // 가장 가까운 드론과 연결  
  CoDrone.DroneModeChange(Flight);     // 드론을 플라이트 모드로 설정합니다. (비행형)
  
  delay(300);
  
  // 밑면 센서 모두 입력으로 사용
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  
  delay(100);
}

void loop()
{
  byte bt1 = digitalRead(11);
  byte bt2 = digitalRead(12);
  byte bt3 = digitalRead(13);
  byte bt4 = digitalRead(14); //14 & 15
  byte bt6 = digitalRead(16);
  byte bt7 = digitalRead(17);
  byte bt8 = digitalRead(18);

  //*************************** Slow Up ***************************//
  //■ □ □ □ □ □ □   1 번째 센서에 손대면 천천히 상승
  if (bt8 && !bt7 && !bt6 && !bt4 && !bt3 && !bt2 && !bt1)
  {
    THROTTLE = slowUp;
    CoDrone.Control();
    delay(slowTime);
    THROTTLE = 0;
    CoDrone.Control();
  }
  
  //*************************** Slow Down ***************************//
  //□ □ □  □ □ □  ■   7 번째 센서에 손대면 천천히 하강
  else if (!bt8 && !bt7 && !bt6 && !bt4 && !bt3 && !bt2 && bt1)
  {
    THROTTLE = slowDown;
    CoDrone.Control();
    delay(slowTime);
    THROTTLE = 0;
    CoDrone.Control();
  }
  
  //**************************** STOP ******************************//
  //□ □ □  ■  □ □ □    밑몉 센서 4번째 감지시 Stop
  else if (bt4)
  {
    CoDrone.FlightEvent(Stop);
  }
  
  //**************************** Control ******************************//
  //□ ■ □ □ □ □ □  밑면 2 번째 센서 감지시 전진 
  //Forward
  else if (!bt8 && bt7 && !bt6 && !bt4 && !bt3 && !bt2 && !bt1)
  {
    PITCH = 40;
    CoDrone.Control();
  }
  
  //□ □ □  □  □ ■ □  밑면 6 번째 센서 감지시 후진 
  //Back
  else if (!bt8 && !bt7 && !bt6 && !bt4 && !bt3 && bt2 && !bt1)
  {
    PITCH = -40;
    CoDrone.Control();
  }
  
  //□ □ ■  □  □ □ □  밑면 3 번째 센서 감지시 왼쪽 
  //Left
  else if (!bt8 && !bt7 && bt6 && !bt4 && !bt3 && !bt2 && !bt1)
  {
    ROLL = -40;
    CoDrone.Control();
  }
  
  //□ □ □  □  ■  □ □    밑면 5 번째 센서 감지시 오른쪽 
  //Right
  else if (!bt8 && !bt7 && !bt6 && !bt4 && bt3 && !bt2 && !bt1)
  {
    ROLL = 40;
    CoDrone.Control();
  }
  
  delay(10);
}
