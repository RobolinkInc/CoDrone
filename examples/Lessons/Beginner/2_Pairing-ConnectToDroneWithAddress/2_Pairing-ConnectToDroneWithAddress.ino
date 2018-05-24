#include<CoDrone.h>

void setup(){
    CoDrone.begin(115200);
    
    byte droneAddress[6] = {0xEC, 0x3F, 0xE5, 0xC2, 0xB5, 0xD0};
      //This will be replaced with your CoDrone's unique address
    CoDrone.AutoConnect(AddressInputDrone, droneAddress);
}

void loop(){
}