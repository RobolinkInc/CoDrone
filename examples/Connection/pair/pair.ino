/*------------------------------------------------------------------
Pair
Objective   : Pairing with drone in three different way
syntax      : pair()
              pair(Nearest)
              pair("address array"); *address array is 6 digits of number
description : This example is showing pair 3 different ways
              pair without parameter is for using saved address in arduino board
              so it will connect with only one drone if you haven't connect
              before it will connect with the strongest signal drone.
              pair(Nearest) is for connect with the strongest signal drone.
              pair("address array") will connect specific drone which has
              specific address
-------------------------------------------------------------------*/
#include<CoDrone.h>     //header
void setup() {
  CoDrone.begin(115200);

  /* 1) This will pair with the last drone that the remote was paired to. If this is the first time pairing, 
   *    this function will pair with whatever first drone it finds, and "lock in" to that drone and only pair with 
   *    that drone
   */
  CoDrone.pair();

  /* 2) This will pair with the nearest drone that the BLE board finds. You can use this function to "unlock" a 
   *    remote that's been "locked in" to a drone. So first run this code, then just upload and run with "CoDrone.pair()" *    to "lock in" to that drone
   */
  //CoDrone.pair(Nearest);

  /* 3) This is a more advanced pairing process that always only pair with the provided drone's address. The code
   *    to print your drone's address is provided below.  The syntax used below is assuming you ran the code below 
   *    and got your drone's address, which in this example is {43, 171, 60, 234, 74, 136}
   */
  //CoDrone.pair({43, 171, 60, 234, 74, 136});
  //Arduino code to print CoDrone address to Serial Monitor
  /* 1) Once you've uploaded this code to your remote, keep the micro USB cable plugged in and go into run mode by
   *    putting all DIP switch down to OFF mode.
   * 2) Open your Serial Monitor in Arduino, then power on your CoDrone, and reset your remote to let it pair.  Once
   *    it pairs, your drone address should print out, which you can use in the code provided for using "CoDrone.
   *    pair(address)"
   */
}
void loop() {

}

