#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "protocol/protocol.h"
#include "../tests/paxample.h"
#include "../tests/showme.h"

//////////////////////ВЫДЕЛЯЕТСЯ ВСЯ ВНУТРЕННЯЯ ПАМЯТЬ И RSSI ПРИ ОТСУТСТВИИ АППАРАТНОГО
WorkTable       RAM;
int             RSSI;
/////////////////////ФУНКЦИЯ СТАРТ ВЫЗЫВАЕТСЯ ЕДИНОЖДЫ ПРИ ЗАПУСКЕ ПРОГРАММЫ
void start(){
    RAM.MAC = 0x69696969;                                                                                               // Мак адрес устройства, в дальнейшем будет случайным
    RAM.Status = SLEEP;                                                                                                 // Первоначальное состояние устройства
    RAM.start_status_time = clock()/CLOCKS_PER_SEC;
}
/////////////////////ОСНОВНОЙ ЦИКЛ МИКРОКОНТРОЛЛЕРА
void loop(){
    //Осн.Менеджер(Данные с датчиков, Адрес основной памяти, пакет)
    switch(getchar()) {
        //отладка
        case'0': PacketManager(SensorData_0, 200, &RAM, PacketType0_1);    break;
        case'1': PacketManager(SensorData_0, 300, &RAM, PacketType0_2);    break;
        case'2': PacketManager(SensorData_0, 200, &RAM, PacketType0_3);    break;
        case'3': PacketManager(SensorData_0, 100, &RAM, PacketType0_4);    break;

        case'6': PacketManager(SensorData_0, RSSI, &RAM, PacketType1_1);    break;
        case'7': PacketManager(SensorData_0, RSSI, &RAM, PacketType1_2);    break;
        case'8': PacketManager(SensorData_0, RSSI, &RAM, PacketType1_3);    break;
        case'9': PacketManager(SensorData_0, RSSI, &RAM, PacketType1_E);    break;

        case'e': PacketManager(SensorData_0, RSSI, &RAM, PacketType0_E);    break;
        case'f': PacketManager(SensorData_0, RSSI, &RAM, PacketType0_E1);   break;
        case'n': PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO);   break;
        case's': ShowRAMTable(&RAM);                                        break;
    }
}
////////////////////ТОЧКА ВХОДА В ПРОГРАММУ
int  main() {
    system("chcp 65001");
    start();
    while(1)
       loop();
}