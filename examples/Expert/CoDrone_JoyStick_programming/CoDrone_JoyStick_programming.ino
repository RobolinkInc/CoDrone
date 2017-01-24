/*   -  CoDrone Hand Programming 사용법  -
 *    PC연결없이 조이스틱만을 사용하여 드론의 순차비행을 코딩합니다.
 *    간단한 조작방법은 아래에 기술되어 있습니다.
 *    자세한 사용법은 온라인 매뉴얼을 참조바랍니다.
 
      ********페어링이 되어야함*******
  -Step1.슬롯 선택
                                  ◎       < ◎ >
  오른쪽 조이스틱으로 좌우 이동              ∨
  밑으로 내리면 선택

  -Step2.프로그래밍

     -시작-                   ◎         ◎
                              ∨         ∨

     -삭제-                 < ◎       < ◎


     -이전값 추가-            ◎ >       ◎ >


     -전체 삭제-              ◎ >     < ◎

  왼쪽 조이스틱 상하 = Throttle + -
  왼쪽 조이스틱 좌우 = Yaw + -
  오른쪽 조이스틱 상하 = Pitch + -
  오른쪽 조이스틱 좌우 = Roll + -
*/

#include <CoDrone.h>
#include <SmartInventor.h>
#include <EEPROM.h>

#define MAXPROGRAM 90   // max 30 * 3 (act,speed,delay) 

#define THROTTLE_UP     1
#define THROTTLE_DOWN   2
#define YAW_UP          3
#define YAW_DOWN        4
#define PITCH_UP        5
#define PITCH_DOWN      6
#define ROLL_UP         7
#define ROLL_DOWN       8
#define DRONE_STOP      10

int dipSw1 = 8;
int dipSw2 = 9;
int dipSw3 = 10;

int programSlot = 1;
int eepromStartAddr;
int eepromCountAddr;
int term;

byte programCount = 0;
byte DroneActMem[MAXPROGRAM];
byte DroneSpeedMem[MAXPROGRAM];
byte DroneDelayMem[MAXPROGRAM];

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)

  CoDrone.AutoConnect(NearbyDrone);     // 가장 가까운 위치의 드론과 연결

  CoDrone.DroneModeChange(Flight);       // 드론을 플라이트 모드로 설정합니다. (비행형)
  pinMode(dipSw1, INPUT_PULLUP);
  pinMode(dipSw2, INPUT_PULLUP);
  pinMode(dipSw3, INPUT_PULLUP);

  DDRC = 0xff;
  for (int i = 0; i < 4; i++)
  {
    PORTC = PORTC | 0b10000000 >> i;
    delay(80);
    PORTC = PORTC | 0b01 << i;
    delay(80);
  }

  for (int i = 1; i < 5; i++)
  {
    PORTC = 0b00001111 >> i | (PORTC & 0xf0);
    delay(80);
    PORTC = 0b11110000 << i | (PORTC & 0xf);;
    delay(80);
  }

  //-------------------------- SELECT SLOT -------------------------------//
  byte selectModeEnd = 0;

  while (!selectModeEnd)
  {
    word PitchJoyValue = analogRead(A6);
    word RollJoyValue = analogRead(A5);

    if (RollJoyValue > 1000)
    {
      SmartInventor.Buzz(1542, 20);
      SmartInventor.Buzz(2315, 16);
      if (programSlot == 1) programSlot = 8;
      else    programSlot--;
      Serial.print("programSlot\t");
      Serial.println(programSlot);
    }

    else if (RollJoyValue < 20)
    {
      SmartInventor.Buzz(1542, 24);
      SmartInventor.Buzz(2315, 20);
      if (programSlot == 8) programSlot = 1;
      else programSlot++;
      Serial.print("programSlot\t");
      Serial.println(programSlot);
    }

    else if (PitchJoyValue < 20)
    {
      selectModeEnd = 1;

      Serial.print("ProgramSlot Select\t");
      Serial.println(programSlot);
      Serial.println();

      SmartInventor.Buzz(1981, 16);
      SmartInventor.Buzz(2132, 16);
      SmartInventor.Buzz(2315, 12);

      int led_sign = 0;
      DDRC =  0xff;
      while (led_sign < 4)
      {
        PORTC = (0b01 << led_sign) | (0b10000000 >> led_sign);
        led_sign++;
        delay(50);
      }

      led_sign = 0;
      while (led_sign < 4)
      {
        PORTC = (0b00010000 << led_sign) | (0b00001000 >> led_sign);
        led_sign++;
        delay(50);
      }
    }

    LedPerformanceNumberZero(programSlot);

    SmartInventor.PreviousMillis = millis();
    while (!SmartInventor.TimeCheckMillis(150));
  }

  //-------------------------- EEPROM READ -------------------------------//

  eepromStartAddr = ((programSlot * 100));
  eepromCountAddr = eepromStartAddr;
  programCount = 0;

  Serial.print("programSlot\t");
  Serial.println(programSlot);

  Serial.print("eepromStartAddr\t");
  Serial.println(eepromStartAddr);

  programCount = EEPROM.read(eepromCountAddr);

  if (programCount > 30) programCount = 0;

  Serial.println();
  Serial.print("programCount\t");
  Serial.println(programCount);
  Serial.println();

  Serial.println("Addr\tAct\tSpeed\tDelay");

  for (int i = 1; i <= programCount; i++)
  {
    int _act = EEPROM.read(eepromStartAddr + (i * 3) - 2);
    int _speed = EEPROM.read(eepromStartAddr + (i * 3) - 1);
    int _delay = EEPROM.read(eepromStartAddr + (i * 3));

    DroneActMem[(i * 3) - 2] = _act;
    DroneSpeedMem[(i * 3) - 1] = _speed;
    DroneDelayMem[(i * 3) - 0] = _delay;

    Serial.print(i);
    Serial.print("\t");
    Serial.print(_act);
    Serial.print("\t");
    Serial.print(_speed);
    Serial.print("\t");
    Serial.println(_delay);
  }

  //----------------------------------------------------------------------//

}

void loop()
{
  delay(500);
  ProgramingLoop();

  //----------------------------------------------------------------------//

  delay(100);                         // 명령후 잠시 대기

  //----------------------------------------------------------------------//
  int runningProgram = 0;
  while (runningProgram < programCount)
  {
    runningProgram++;
    LedPerformanceNumberZero(runningProgram);
    //   Serial.println(runningProgram);

    byte _act = DroneActMem[(runningProgram * 3) - 2];

    if (_act == 1) // Throttle Up
    {
      THROTTLE  = (DroneSpeedMem[(runningProgram * 3) - 1]) * 10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 2) // Throttle Down
    {
      THROTTLE  = (DroneSpeedMem[(runningProgram * 3) - 1]) * -10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 3) // Yaw Up  (Turn Right)
    {
      YAW  = (2 + DroneSpeedMem[(runningProgram * 3) - 1]) * 10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 4) // Yaw Down  (Turn Left)
    {
      YAW  = (DroneSpeedMem[(runningProgram * 3) - 1]) * -10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 5) // Pitch Up (Forward)
    {
      PITCH  = (DroneSpeedMem[(runningProgram * 3) - 1]) * 10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 6) // Pitch Down (Backward)
    {
      PITCH  = (DroneSpeedMem[(runningProgram * 3) - 1]) * -10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 7) // Roll Up (Move Right)
    {
      ROLL  = (DroneSpeedMem[(runningProgram * 3) - 1]) * 10;
      CoDrone.Control();                   // 조종값 전송
    }

    else if (_act == 8) // Roll Down (Move Left)
    {
      ROLL  = (DroneSpeedMem[(runningProgram * 3) - 1]) * -10;
      CoDrone.Control();                   // 조종값 전송
    }

    SmartInventor.PreviousMillis = millis();

    while (!SmartInventor.TimeCheckMillis(DroneDelayMem[runningProgram * 3] * 100))
    {
      word ThrottleJoyValue = analogRead(A4);
      word PitchJoyValue = analogRead(A6);

      while (ThrottleJoyValue < 220 && PitchJoyValue < 220)    //emergency Stop ▼▼ (all down)
      {
        ThrottleJoyValue = analogRead(A4);
        PitchJoyValue = analogRead(A6);
        CoDrone.FlightEvent(Stop);      // 드론을 정지시킵니다.
        SmartInventor.Buzz(2400, 12);
        delay(500);
        runningProgram = programCount;
      }
    }
  }

  CoDrone.FlightEvent(Stop);      // 드론을 정지시킵니다.

  while ((analogRead(A4) < 412) || (analogRead(A6) < 412));

}

void DroneProgramProcess(byte sensor, byte direction, int Limit)
{
  unsigned long _oldTime = millis();
  int _speed;
  int _delay;
  int upDownDir = 0;
  int ledMoveTime  = 350;
  int speedOffset = 2;

  if (Limit > 0)
  {
    //-------------------------- Direction & Speed --------------------------------//
    // direction speed
    while (analogRead(sensor) > Limit)
    {
      _speed = ((millis() - _oldTime) / ledMoveTime);
      upDownDir = _speed / 8;
      if (upDownDir % 2)  _speed = 8 - (_speed - (upDownDir * 8)); //if 1 ,1,3,5,7<<
      else                _speed = _speed - (upDownDir * 8); //if 0 2,4,6,8 >>

      for (int thisPin = 11; thisPin <= 18; thisPin++)
      {
        if (thisPin <= _speed + 10) digitalWrite(thisPin, HIGH);
        else   digitalWrite(thisPin, LOW);
      }
    }
    SmartInventor.Buzz(1984, 12);

    //------------------------------ Delay ----------------------------------------//
    _oldTime = millis();
    while (analogRead(sensor) < Limit + 100)
    {
      _delay = ((millis() - _oldTime) / ledMoveTime);
      upDownDir = _delay / 8;

      if (upDownDir % 2)  _delay =  (_delay - (upDownDir * 8));       //if 1 ,1,3,5,7<<
      else                _delay = 8 - (_delay - (upDownDir * 8));    //if 0 2,4,6,8 >>

      for (int thisPin = 11; thisPin <= 18; thisPin++)
      {
        if (thisPin <= _delay + 10) digitalWrite(thisPin, LOW);
        else   digitalWrite(thisPin, HIGH);
      }
      _delay = 8 - _delay;
    }
    //-----------------------------------------------------------------------//
    //wait - basice pos
    while (analogRead(sensor) > Limit - 100);
  }

  else if (Limit < 0)
  {
    Limit = Limit * -1;
    // direction speed
    while (analogRead(sensor) < Limit)
    {
      _speed = ((millis() - _oldTime) / ledMoveTime);
      upDownDir = _speed / 8;
      if (upDownDir % 2)  _speed = 8 - (_speed - (upDownDir * 8)); //if 1 ,1,3,5,7<<
      else                _speed = _speed - (upDownDir * 8); //if 0 2,4,6,8 >>

      for (int thisPin = 11; thisPin <= 18; thisPin++)
      {
        if (thisPin <= _speed + 10) digitalWrite(thisPin, HIGH);
        else   digitalWrite(thisPin, LOW);
      }
    }
    SmartInventor.Buzz(1984, 12);
    // delay
    _oldTime = millis();
    //-----------------------------------------------------------------------//
    while (analogRead(sensor) > Limit - 100)
    {
      _delay = ((millis() - _oldTime) / ledMoveTime);
      upDownDir = _delay / 8;

      if (upDownDir % 2)  _delay =  (_delay - (upDownDir * 8));       //if 1 ,1,3,5,7<<
      else                _delay = 8 - (_delay - (upDownDir * 8));    //if 0 2,4,6,8 >>

      for (int thisPin = 11; thisPin <= 18; thisPin++)
      {
        if (thisPin <= _delay + 10) digitalWrite(thisPin, LOW);
        else   digitalWrite(thisPin, HIGH);
      }
      _delay = 8 - _delay;
    }
    //-----------------------------------------------------------------------//

    //wait - basice pos
    while (analogRead(sensor) < Limit + 100);
  }

  programCount++;
  DroneActMem[(programCount * 3) - 2] = direction;
  DroneSpeedMem[(programCount * 3) - 1] = _speed + speedOffset;
  DroneDelayMem[programCount * 3] = _delay;

  Serial.println();

  Serial.print(programCount);
  Serial.print("\t");
  Serial.print(direction);
  Serial.print("\t");
  Serial.print(_speed + speedOffset);
  Serial.print("\t");
  Serial.println(_delay);

  EEPROM.write(eepromStartAddr + ((programCount * 3) - 2), direction);
  EEPROM.write(eepromStartAddr + ((programCount * 3) - 1), _speed + speedOffset);
  EEPROM.write(eepromStartAddr + ((programCount * 3)), _delay);
  EEPROM.write(eepromCountAddr, programCount);

  SmartInventor.Buzz(1504, 12);
  SmartInventor.Buzz(740, 12);
}


void ProgramingLoop()
{
  byte programingEnd = 0;

  //Led Output
  for (int thisPin = 11; thisPin <= 18; thisPin++)   {
    pinMode(thisPin, OUTPUT);
  }

  while (!programingEnd)
  {
    word  selectLevel0 = SmartInventor.SensorCalibration(A0) - 300;
    word  selectLevel1 = SmartInventor.SensorCalibration(A1) - 300;
    word  selectLevel2 = SmartInventor.SensorCalibration(A2) - 300;

    word leftSensorValue = analogRead(A0);
    word centerSensorValue = analogRead(A1);
    word rightSensorValue = analogRead(A2);
    word ThrottleJoyValue = analogRead(A4);
    word YawJoyValue = analogRead(A3);
    word PitchJoyValue = analogRead(A6);
    word RollJoyValue = analogRead(A5);

    LedPerformanceNumberZero(programCount);

    if ( SmartInventor.TimeCheckMillis(400))
    {
      if ((YawJoyValue > 920) && (RollJoyValue > 920)) // delete Action
      {
        if (programCount > 0)
        {
          programCount--;

          Serial.println();
          Serial.print(programCount);
          Serial.println("\tDelete Action");

          EEPROM.write(eepromCountAddr, programCount);
          SmartInventor.Buzz(1160, 12);
        }
        while ((YawJoyValue > 612) || (RollJoyValue > 612))
        {
          YawJoyValue = analogRead(A3);
          RollJoyValue = analogRead(A5);
        }
      }

      else if ((PitchJoyValue < 220) && (ThrottleJoyValue < 220)) //start
      {
        programingEnd = 1;

        Serial.println();
        Serial.print(programCount);
        Serial.println("\tProgram Start");

        SmartInventor.Buzz(1981, 12);
        SmartInventor.Buzz(2132, 12);
        SmartInventor.Buzz(2315, 12);
        delay(500);
        while ((PitchJoyValue < 412) || (ThrottleJoyValue < 412))
        {
          ThrottleJoyValue = analogRead(A4);
          PitchJoyValue = analogRead(A6);
        }
      }
      else if ((YawJoyValue < 120) && (RollJoyValue > 920))  //all delete  -->> <<--
      {
        programCount = 0;
        LedPerformanceNumberZero(programCount);
        EEPROM.write(eepromCountAddr, programCount);

        Serial.println();
        Serial.print(programCount);
        Serial.println("\tDelete ALL");

        SmartInventor.Buzz(1984, 12);
        SmartInventor.Buzz(2016, 12);

        while ((YawJoyValue < 412) || (RollJoyValue > 612))
        {
          YawJoyValue = analogRead(A3);
          RollJoyValue = analogRead(A5);
        }
      }

      else if (ThrottleJoyValue < 220 && PitchJoyValue < 220)    //emergency Stop ▼▼ (all down)
      {
        while (ThrottleJoyValue < 220 && PitchJoyValue < 220)
        {
          ThrottleJoyValue = analogRead(A4);
          PitchJoyValue = analogRead(A6);

          CoDrone.FlightEvent(Stop);      // 드론을 정지시킵니다.
          SmartInventor.Buzz(2400, 12);

          delay(500);
          Serial.println("Emergency Stop");
          delay(10);
        }

        while ((ThrottleJoyValue < 412) || (PitchJoyValue < 412))
        {
          ThrottleJoyValue = analogRead(A4);
          PitchJoyValue = analogRead(A6);
        }
      }
      if ((YawJoyValue < 120) && (RollJoyValue < 120)) // Repeat the last Action
      {
        programCount++;

        Serial.println();
        Serial.print(programCount);
        Serial.println("\tRepeat the last Action");

        DroneActMem[(programCount * 3) - 2]   = DroneActMem[((programCount - 1) * 3) - 2];
        DroneSpeedMem[(programCount * 3) - 1] = DroneSpeedMem[((programCount - 1) * 3) - 1] ;
        DroneDelayMem[programCount * 3]       = DroneDelayMem[(programCount - 1) * 3];

        EEPROM.write(eepromStartAddr + ((programCount * 3) - 2), DRONE_STOP);
        EEPROM.write(eepromStartAddr + ((programCount * 3) - 1), 0);
        EEPROM.write(eepromStartAddr + ((programCount * 3)), 0);
        EEPROM.write(eepromCountAddr, programCount);

        SmartInventor.Buzz(1160, 12);
        while ((YawJoyValue < 412) || (RollJoyValue < 412))
        {
          YawJoyValue = analogRead(A3);
          RollJoyValue = analogRead(A5);
        }

      }
      //------------------------------------------------------------------------------------------------------//
      if (programCount < MAXPROGRAM / 3)
      {
        if (PitchJoyValue > 950) {
          term += 4;
          //   Serial.println(term);
          if (term == 8) {
            DroneProgramProcess(A6, PITCH_UP, 900); //forward
            term = 0;
          }
        }
        if (PitchJoyValue < 70) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A6, PITCH_DOWN, -123); //back
            term = 0;
          }
        }

        if (YawJoyValue > 950) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A3, YAW_DOWN, 900); //turn back
            term = 0;
          }
        }
        if (YawJoyValue < 70) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A3, YAW_UP, -123); //right turn
            term = 0;
          }
        }

        if (RollJoyValue > 950) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A5, ROLL_DOWN, 900); //left turn
            term = 0;
          }
        }
        if (RollJoyValue < 70) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A5, ROLL_UP, -123); //left turn
            term = 0;
          }
        }

        if (ThrottleJoyValue > 950) {
          term += 4;
          if (term == 8) {
            DroneProgramProcess(A4, THROTTLE_UP, 900); //throttle up
            term = 0;
          }
        }
        if (ThrottleJoyValue < 70) {
          term += 4; {
            DroneProgramProcess(A4, THROTTLE_DOWN, -123); //throttle down
            term = 0;
          }
        }
      }
    }
  }
}

void LedPerformanceNumberZero(byte num)
{
  DDRC = 0xff;

  if (num >= 27)
  {
    PORTC = 240;
    num = num - 26;
  }
  else if (num >= 22)
  {
    PORTC = 224;
    num = num - 21;
  }
  else if (num >= 16)
  {
    PORTC = 192;
    num = num - 15;
  }
  else if (num >= 9)
  {
    PORTC = 128;
    num = num - 8;
  }
  else
  {
    PORTC = 0;
  }

  if (num == 0) PORTC =  0;
  else
  {
    PORTC = (1 << num - 1) | PORTC;
  }
}



