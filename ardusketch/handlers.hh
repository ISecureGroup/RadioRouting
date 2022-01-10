
#ifndef HANDLERS_HH
#define HANDLERS_HH
#include "protocol.hh"

//------------------------HANDLERS--------------------------
void 			pl_Handler_00(WorkTable * ram,Packet pack, int RSSI);                                                       //ОБРАБОТЧИК ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void			pl_Handler_01(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я УЗЕЛ"
void 			pl_Handler_02(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void 			pl_Handler_03(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я РОУТЕР"
void 			pl_Handler_04(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОПРОС УСТРОЙСТВ"
void 			pl_Handler_05(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void 			pl_Handler_06(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"
#endif //RADIOROUTING_HANDLERS_H
