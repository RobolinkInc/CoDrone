/*****************************************************************
  - Connect 3
  AutoConnect(droneAddress)
  자동 연결 (드론 어드레스 주소)
*******************************************************************/

#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  // CoDrone.PrintDroneAddress();        // 최근에 연결한 드론의 어드레스 주소를 모니터로 출력합니다.

  byte droneAddress[6] = {0xC, 0xB8, 0x3C, 0xEA, 0x4A, 0x81};  // 어드레스 주소를 입력 - PrintDroneAddress() 명령으로 확인)
  CoDrone.AutoConnect(AddressInputDrone, droneAddress);         // 입력한 어드레스 주소와 같은 드론과 연결
  
  CoDrone.DroneModeChange(Flight);       // 드론을 플라이트 모드로 설정합니다. (비행형)
  
  // 2초간 드론을 띄운후 착륙하기
  if (PAIRING == true)                    // 연결(페어링)이 성공한 경우에만 실행
  {
    CoDrone.FlightEvent(TakeOff);        // 이륙
    delay(2000);                          // 대기 시간
    CoDrone.FlightEvent(Landing);        // 서서히 착륙
  }
}

void loop()
{
}


