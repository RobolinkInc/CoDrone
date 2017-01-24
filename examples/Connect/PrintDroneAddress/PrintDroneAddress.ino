/*****************************************************************
  PrintDroneAddress // Connected Address Confirm
  최근에 연결한 드론의 어드레스 주소를 시리얼 모니터로 출력
    AutoConnect(droneAddress) 입력시에 사용합니다.
*******************************************************************/

#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.PrintDroneAddress();          // 최근에 연결한 드론의 어드레스 주소를 시리얼 모니터로 출력합니다.
}

void loop()
{

}


