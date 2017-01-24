/*****************************************************************
  Trim - Serial Trim Set

-드론이 한쪽으로 치우치거나 출력의 제어가 안되는 경우에는 트림 기능을 사용하여 보정합니다.
  드론의 트림 값을 시리얼 통신을 통해서 설정합니다.

  드론을 켠 상태에서 PC와 보드를 USB로 연결합니다.
  아두이노 창의 시리얼 모니터를 열고 115200bps, Newline 으로 설정합니다.
  화면에 나오는 명령어를 입력하면 해당명령이 실행됩니다.

  # 개별 명령 : 개별 명령은 명령어만 입력하면 작동합니다.
  help  : 명령어를 화면에 다시 출력합니다.
  now   : 현재 드론의 트림을 확인합니다.
  reset : 드론의 전체 트림 값을 기본으로 설정합니다.(0으로 설정)

  # 설정 명령 : 설정 명령은 명령어와 설정할 수치 값을 같이 입력해야 합니다.
   예를 들어 roll을 20으로 변경하는 경우,
  :"roll 20" 과 같이 roll 명령 후에 한칸을 띄운 후 수치를 입력합니다.

    ex) throttle -70
        pitch 40
        yaw -20

  모든 값은 -100 ~ 100으로 설정하며, 초기 기준 값은 0입니다.

  roll [value] : roll 값을 설정합니다.
  yaw  [value] : yaw 값을 설정합니다.
  pitch [value]: pitch 값을 성정합니다.
  throttle [value] : throttle 값을 설정합니다.
  wheel [value]: wheel 값을 설정합니다. (드라이브 모드에서 적용)

*******************************************************************/

#include <CoDrone.h> // 코드론을 사용하기 위한 헤더파일 

#define MAX_CMD_LENGTH  50
char cmdBuff[MAX_CMD_LENGTH];
int cmdIndex = 0;
char* cmd;
char* arg1;
char* arg2;

#define _Roll       1
#define _Yaw        2
#define _Throttle   3
#define _Pitch      4
#define _Wheel      5
byte command = 0;


long ver = 161111;

void setup()
{
  CoDrone.begin(115200);                // 드론 플러그의 통신 개시 (115200bps)
  CoDrone.AutoConnect(NearbyDrone);    // 가장 가까운 위치의 드론과 연결

  PrintTrimAll();                        // 현재 트림값 표시 및 변수에 저장

  delay(500);
  CoDrone.Send_LinkModeBroadcast(LinkModeMute);
  delay(500);

  PrintMenu();
  printPrompt();
}

void loop()
{
  if (Serial.available() > 0)
  {
    int input = Serial.read();
    Serial.write(input); // echo
    if (input == '\r')
    {
      input = '\n';
      Serial.write(input); // echo
    }
    cmdBuff[cmdIndex++] = (char)input;
    if (cmdIndex >= MAX_CMD_LENGTH)
    {
      cmdIndex = 0;
      printMaxLengthError();
    }
    else
    {
      if (cmdBuff[cmdIndex - 1] == '\n')
      {
        cmdBuff[cmdIndex - 1] = 0; // end of string
        cmd = strtok(cmdBuff, " "); // spilt command
        arg1 = strtok(0, " "); // spilt argument1
        arg2 = strtok(0, " "); // spilt argument2

        if (strcmp(cmd, "help") == 0)
          PrintMenu();

        else if (strcmp(cmd, "now") == 0)
        {
          delay(500);
          CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active);
          PrintTrimAll();
        }

        else if (strcmp(cmd, "reset") == 0)
          cmdTrimReset();

        else if (strcmp(cmd, "roll") == 0)
        {
          command = _Roll;
          cmdTrimSet(arg1);
        }
        else if (strcmp(cmd, "pitch") == 0)
        {
          command = _Pitch;
          cmdTrimSet(arg1);
        }
        else if (strcmp(cmd, "yaw") == 0)
        {
          command = _Yaw;
          cmdTrimSet(arg1);
        }
        else if (strcmp(cmd, "throttle") == 0)
        {
          command = _Throttle;
          cmdTrimSet(arg1);
        }
        else if (strcmp(cmd, "wheel") == 0)
        {
          command = _Wheel;
          cmdTrimSet(arg1);
        }

        else
        {
          if (cmdIndex - 1 == 0)
            printPrompt();
          else
            printCmdError();
        }
        cmdIndex = 0;
      }
    }
  }
}

void cmdTrimReset()
{
  delay(500);
  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active);
  CoDrone.Set_TrimAll(0, 0, 0, 0, 0);
  PrintTrimAll();
}

void cmdTrimSet(char* arg1)
{
  int _Trim = atoi(arg1);

  delay(500);

  CoDrone.Send_LinkModeBroadcast(LinkBroadcast_Active);

  delay(500);
  if (command == _Roll)
    CoDrone.Set_TrimAll(_Trim, CoDrone.TrimAll_Pitch, CoDrone.TrimAll_Yaw, CoDrone.TrimAll_Throttle, CoDrone.TrimAll_Wheel);
  else if (command == _Pitch)
    CoDrone.Set_TrimAll(CoDrone.TrimAll_Roll, _Trim, CoDrone.TrimAll_Yaw, CoDrone.TrimAll_Throttle, CoDrone.TrimAll_Wheel);
  else if (command == _Yaw)
    CoDrone.Set_TrimAll(CoDrone.TrimAll_Roll, CoDrone.TrimAll_Pitch, _Trim, CoDrone.TrimAll_Throttle, CoDrone.TrimAll_Wheel);
  else if (command == _Throttle)
    CoDrone.Set_TrimAll(CoDrone.TrimAll_Roll, CoDrone.TrimAll_Pitch, CoDrone.TrimAll_Yaw, _Trim, CoDrone.TrimAll_Wheel);
  else if (command == _Wheel)
    CoDrone.Set_TrimAll(CoDrone.TrimAll_Roll, CoDrone.TrimAll_Pitch, CoDrone.TrimAll_Yaw, CoDrone.TrimAll_Throttle, _Trim);

  command = 0;
  PrintTrimAll();

}


void PrintMenu()
{

  while (Serial.available() > 0)
  {
    byte Temp = Serial.read();
  }

  Serial.println();
  Serial.println("--------------- Trim Set Mode --------------- ");
  Serial.print("- version = ");
  Serial.println(ver);
  Serial.println("- Serial Monitor Mode => Newline, 115200 baud");
  Serial.println("  Value Range -500 ~ 500");
  Serial.println();
  Serial.println("help");
  Serial.println("now");
  Serial.println("reset");
  Serial.println("roll[value]\t EX : roll 20");
  Serial.println("pitch[value]\t EX : pitch -20");
  Serial.println("yaw[value]\t EX : yaw 20");
  Serial.println("throttle[value]\t EX : throttle -20");
  Serial.println("wheel[value]\t EX : wheel 20");
  Serial.println("--------------------------------------------- ");

  printPrompt();
}

void PrintTrimAll()
{
  delay(1000);

  CoDrone.Request_TrimAll();

  CoDrone.PreviousMillis = millis();
  while (!CoDrone.TimeCheck(100))
  {
    CoDrone.Receive();
  }

  CoDrone.Send_LinkModeBroadcast(LinkModeMute);
  delay(500);

  Serial.println("");
  Serial.println("");
  Serial.println("--------------- Now Trim ---------------");

  Serial.print("roll \t\t");
  Serial.println(CoDrone.TrimAll_Roll);

  Serial.print("pitch \t\t");
  Serial.println(CoDrone.TrimAll_Pitch);

  Serial.print("yaw \t\t");
  Serial.println(CoDrone.TrimAll_Yaw);

  Serial.print("throttle \t");
  Serial.println(CoDrone.TrimAll_Throttle);

  Serial.print("wheel \t\t");
  Serial.println(CoDrone.TrimAll_Wheel);
  Serial.println("--------------------------------------------- ");

  delay(1000);
  while (Serial.available() > 0)
  {
    byte Temp = Serial.read();
    //       Serial.println(Temp,HEX);
  }
  printPrompt();
}

void printCmdError()
{
  Serial.println("Bad command. type 'help'");
  printPrompt();
}
void printPrompt()
{
  Serial.print("> ");
}
void printMaxLengthError()
{
  Serial.println();
  Serial.println("Max command length");
  printPrompt();
}

