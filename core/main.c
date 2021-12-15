#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "protocol/protocol.h"
#include "../tests/paxample.h"


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
        case'0': PacketManager(SensorData_0, RSSI, &RAM, PacketType0_1);    break;
        case'1': PacketManager(SensorData_0, RSSI, &RAM, PacketType0_2);    break;
        case'a': PacketManager(SensorData_0, RSSI, &RAM, PacketType1_1);    break;
        case'2': PacketManager(SensorData_0, RSSI, &RAM, PacketType2_1);    break;
        case'3': PacketManager(SensorData_0, RSSI, &RAM, PacketType3_1);    break;
        case'4': PacketManager(SensorData_0, RSSI, &RAM, PacketType4_1);    break;
        case'5': PacketManager(SensorData_0, RSSI, &RAM, PacketType5_1);    break;
        case'6': PacketManager(SensorData_0, RSSI, &RAM, PacketType6_1);    break;
        case'#': PacketManager(SensorData_0, RSSI, &RAM, PacketType0_1);    break;
        case'c': PacketManager(SensorData_0, RSSI, &RAM, PacketType2_1);    break;
        case'e': PacketManager(SensorData_0, RSSI, &RAM, PacketType2_1);    break;


    }
}
////////////////////ТОЧКА ВХОДА В ПРОГРАММУ
int main() {
    system("chcp 65001");
    start();
    while(1)
       loop();
}