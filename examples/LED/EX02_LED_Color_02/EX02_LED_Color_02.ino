/*****************************************************************
  - LED Color 2
  드론의 LED를 제어합니다.
  LedColor(Mode,R,G,B,Time); 모드, R,G,B, 시간의 형식으로 입력합니다.
    모드는 아래의 모드 테이블 참조
    색상은 R,G,B 값을 직접 입력하며 각각 0~255 까지 입력 가능합니다.
    시간은 0~255 까지 입력 가능합니다. (Mode에 따라 다르게 적용)
*******************************************************************/
#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

byte modeTime = 7;                        // 모드 시간 변수
int delayTime = 1000;                     // 대기 시간 변수

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NearbyDrone);    // 가장 가까운 위치의 드론과 연결
}

void loop()
{
  CoDrone.LedColor(ArmDimming, 255, 0, 0, modeTime);   // 입력된 R,G,B 색으로 밝기 제어하여 천천히 깜빡이며  modeTime 따라 동작합니다.
  delay(delayTime);                                     // 대기 시간 입력

  CoDrone.LedColor(ArmDimming, 0, 255, 0, modeTime);   // 입력된 R,G,B 색으로 밝기 제어하여 천천히 깜빡이며  modeTime 따라 동작합니다.
  delay(delayTime);                                     // 대기 시간 입력

  CoDrone.LedColor(ArmDimming, 0, 0, 255, modeTime);   // 입력된 R,G,B 색으로 밝기 제어하여 천천히 깜빡이며  modeTime 따라 동작합니다.
  delay(delayTime);                                     // 대기 시간 입력

  CoDrone.LedColor(ArmDimming, 0, 0, 0, modeTime);     // 입력된 R,G,B 색으로 밝기 제어하여 천천히 깜빡이며  modeTime 따라 동작합니다.
  delay(delayTime);                                     // 대기 시간 입력
}

/*********************************************************************************************************************************
    mode - TABLE
  EyeNone
  EyeHold,          ///< 지정한 색상을 계속 켬
  EyeMix,           ///< 순차적으로 LED 색 변경
  EyeFlicker,       ///< 깜빡임
  EyeFlickerDouble, ///< 깜빡임(두 번 깜빡이고 깜빡인 시간만큼 꺼짐)
  EyeDimming,       ///< 밝기 제어하여 천천히 깜빡임

  ArmNone
  ArmHold,           ///< 지정한 색상을 계속 켬
  ArmMix,            ///< 순차적으로 LED 색 변경
  ArmFlicker,        ///< 깜빡임
  ArmFlickerDouble,  ///< 깜빡임(두 번 깜빡이고 깜빡인 시간만큼 꺼짐)
  ArmDimming,        ///< 밝기 제어하여 천천히 깜빡임
  ArmFlow,           ///< 앞에서 뒤로 흐름
  ArmFlowReverse,    ///< 뒤에서 앞으로 흐름
  *********************************************************************************************************************************/
