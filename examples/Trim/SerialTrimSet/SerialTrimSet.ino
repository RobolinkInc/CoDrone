/*****************************************************************
  Trim - Serial Trim Set

This example will allow you to set your CoDrone trim using your serial monitor.

- After uploading the code, keep your controller plugged into the computer and 
open the Serial Monitor (ctrl + shift + M).
- Set the baud rate to 115200 and the line ending to NewLine
- After the CoDrone is paired, a list of commands and current trim values will appear
on screen

  Type these commands into the Serial input (at the top) to set your trim
  help  : Display a list of commands
  now   : Display current trim values
  reset : Set all trim values to 0

  To set a specific trim, type in the control and the amount:
  :"roll 20" will set your roll trim to 20.  This will lean the drone to the right. 

    ex) throttle -70
        pitch 40
        yaw -20

  Values can range from -100 to 100

  roll [value] : roll trim
  yaw  [value] : yaw trim.
  pitch [value]: pitch trim.
  throttle [value] : throttle trim.

*******************************************************************/

#include <CoDrone.h> // Include CoDrone Library

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

int TrimAll_Roll;
int TrimAll_Pitch;
int TrimAll_Yaw;
int TrimAll_Throttle;
int TrimAll_Wheel;

long ver = 160518;

void setup()
{
  CoDrone.begin(115200);                // Begin serial connection (115200bps)
  CoDrone.AutoConnect(NearbyDrone);    // Connect with a nearby drone

  PrintTrimAll();                        // Display all trim values

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
    CoDrone.Set_TrimAll(_Trim, TrimAll_Pitch, TrimAll_Yaw, TrimAll_Throttle, TrimAll_Wheel);
  else if (command == _Pitch)
    CoDrone.Set_TrimAll(TrimAll_Roll, _Trim, TrimAll_Yaw, TrimAll_Throttle, TrimAll_Wheel);
  else if (command == _Yaw)
    CoDrone.Set_TrimAll(TrimAll_Roll, TrimAll_Pitch, _Trim, TrimAll_Throttle, TrimAll_Wheel);
  else if (command == _Throttle)
    CoDrone.Set_TrimAll(TrimAll_Roll, TrimAll_Pitch, TrimAll_Yaw, _Trim, TrimAll_Wheel);
  else if (command == _Wheel)
    CoDrone.Set_TrimAll(TrimAll_Roll, TrimAll_Pitch, TrimAll_Yaw, TrimAll_Throttle, _Trim);

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
  delay(500);

  CoDrone.Request_TrimAll();

  CoDrone.PreviousMillis = millis();
  while (!CoDrone.TimeCheck(1000))
  {
    CoDrone.Receive();
  }

  CoDrone.Send_LinkModeBroadcast(LinkModeMute);
  delay(500);

  TrimAll_Roll      = ((CoDrone.droneTrimAll[1] << 8) | (CoDrone.droneTrimAll[0]  & 0xff));
  TrimAll_Pitch     = ((CoDrone.droneTrimAll[3] << 8) | (CoDrone.droneTrimAll[2]  & 0xff));
  TrimAll_Yaw       = ((CoDrone.droneTrimAll[5] << 8) | (CoDrone.droneTrimAll[4]  & 0xff));
  TrimAll_Throttle  = ((CoDrone.droneTrimAll[7] << 8) | (CoDrone.droneTrimAll[6]  & 0xff));
  TrimAll_Wheel     = ((CoDrone.droneTrimAll[9] << 8) | (CoDrone.droneTrimAll[8]  & 0xff));

  Serial.println("");
  Serial.println("");
  Serial.println("--------------- Now Trim ---------------");

  Serial.print("roll \t\t");
  Serial.println(TrimAll_Roll);

  Serial.print("pitch \t\t");
  Serial.println(TrimAll_Pitch);

  Serial.print("yaw \t\t");
  Serial.println(TrimAll_Yaw);

  Serial.print("throttle \t");
  Serial.println(TrimAll_Throttle);

  Serial.print("wheel \t\t");
  Serial.println(TrimAll_Wheel);
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

