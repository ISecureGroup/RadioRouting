#include <stdlib.h>
#include "protocol.h"
#include "../tests/paxample.h"


///////////////////////////////////////
WorkTable       RAM;
int             RSSI;
///////////////////////////////////////
void start(){
    RAM.MAC = 0x69696969;
    RAM.STATUS = SLEEP;
    PacketManager(SensorData_0, RSSI, &RAM, PacketType0_1);
    PacketManager(SensorData_0, RSSI, &RAM, PacketType1);
    PacketManager(SensorData_0, RSSI, &RAM, PacketType2_0);
    PacketManager(SensorData_0, RSSI, &RAM, PacketType2_1);
    PacketManager(SensorData_0, RSSI, &RAM, PacketType3);
    PacketManager(SensorData_0, RSSI, &RAM, PacketType4);
    PacketManager(SensorData_1, RSSI, &RAM, PacketType5_0);
    PacketManager(SensorData_1, RSSI, &RAM, PacketType5_1);
    PacketManager(SensorData_1, RSSI, &RAM, PacketType6);
}
void loop(){

}
///////////////////////////////////////
int main() {
    system("chcp 65001");
    start();
   // while(true)
   //     loop();
}