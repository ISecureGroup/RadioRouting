#include <stdlib.h>
#include "protocol.h"
#include "../tests/paxample.h"


//////////////////////ВЫДЕЛЯЕТСЯ ВСЯ ВНУТРЕННЯЯ ПАМЯТЬ И RSSI ПРИ ОТСУТСТВИИ АППАРАТНОГО
WorkTable       RAM;
int             RSSI;
/////////////////////ФУНКЦИЯ СТАРТ ВЫЗЫВАЕТСЯ ЕДИНОЖДЫ ПРИ ЗАПУСКЕ ПРОГРАММЫ
void start(){
    RAM.MAC = 0x69696969;                                                                                               // Мак адрес устройства, в дальнейшем будет случайным
    RAM.STATUS = SLEEP;                                                                                                 // Первоначальное состояние устройства
    PacketManager(SensorData_0, RSSI, &RAM, PacketType0_1);
}
/////////////////////ОСНОВНОЙ ЦИКЛ МИКРОКОНТРОЛЛЕРА
void loop(){
    //Осн.Менеджер(Данные с датчиков, Адрес основной памяти, пакет)
    //PacketManager(SensorData_0, RSSI, &RAM, PacketType0_1);
}
////////////////////ТОЧКА ВХОДА В ПРОГРАММУ
int main() {
    system("chcp 65001");
    start();
   // while(true)
   //     loop();
}