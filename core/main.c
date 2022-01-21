#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include "protocol/protocol.h"
#include "../tests/paxample.h"
#include "../tests/showme.h"

//////////////////////ВЫДЕЛЯЕТСЯ ВСЯ ВНУТРЕННЯЯ ПАМЯТЬ И RSSI ПРИ ОТСУТСТВИИ АППАРАТНОГО
WorkTable       RAM;
int             RSSI;
/////////////////////ФУНКЦИЯ СТАРТ ВЫЗЫВАЕТСЯ ЕДИНОЖДЫ ПРИ ЗАПУСКЕ ПРОГРАММЫ
void start(){
    RAM.MAC = 0x69696969; //GetRandomAddress();                                                                                               // Мак адрес устройства, в дальнейшем будет случайным
    RAM.Status = SLEEP;                                                                                                 // Первоначальное состояние устройства
    RAM.start_status_time = clock()/CLOCKS_PER_SEC;
    printf("Мой адресс: ");
    PrintAddress(RAM.MAC);
}
/////////////////////ОСНОВНОЙ ЦИКЛ МИКРОКОНТРОЛЛЕРА
void loop()
{
        // В SLEEP
        PacketManager(SensorData_0, 200, &RAM, PacketType0_1);
        PacketManager(SensorData_0, 300, &RAM, PacketType0_2);
        PacketManager(SensorData_0, 200, &RAM, PacketType0_3);
        PacketManager(SensorData_0, 100, &RAM, PacketType0_4);
        ShowRAMTable(&RAM);
        Sleep(12000);
        // SEND_01
        // START_DEF_ROUTERS
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_1);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_2);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_3);
        ShowRAMTable(&RAM);
        Sleep(12000);
        //SEND 02

        //WAIT CONFIRM FROM POTENTIAL ROUTER
        //ANNOUNCEMENT POTENTIAL ROUTER STATUS
        //WAITING CONFIRM ROUTER STATUS FROM DEVICES
        //ADDITIONAL WAITING CONFIRM ROUTER STATUS FROM DEVICES
        //READY
        Sleep(1500);

}
////////////////////ТОЧКА ВХОДА В ПРОГРАММУ
int  main() {
    system("chcp 65001");
    start();
    while(1)
       loop();
}