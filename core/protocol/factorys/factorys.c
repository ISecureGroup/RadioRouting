//
// Created by sb12 on 09.12.2021.
//
#include "../protocol.h"

void packet_Factory_00(WorkTable * ram){
    packetConstructor(ram,
                      '$',              //$
                      0x00,             //ТИП ПАКЕТА
                      ram->MAC,                   //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,   //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,               //ВРЕМЯ
                      ram->my_level,              //УРОВЕНЬ
                      ram->my_session,            //СЕССИЯ
                      0x00,                //СЕАНС
                      ram->Device,                //РОЛЬ МОДУЛЯ
                      0x00,            //НОМЕР ПАКЕТА
                      0x01,                   //TTL
                      0x55555555,       //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,       //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,              //РЕЗЕРВ
                      ram->output_payload);       //ПОЛЕЗНАЯ НАГРУЗКА
}
void packet_Factory_01(WorkTable * ram){
    packetConstructor(ram,
                      '$',              //$
                      0x01,             //ТИП ПАКЕТА
                      ram->MAC,                   //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,   //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,               //ВРЕМЯ
                      ram->my_level,              //УРОВЕНЬ
                      ram->my_session,            //СЕССИЯ
                      0x00,                //СЕАНС
                      0x00,              //РОЛЬ МОДУЛЯ
                      ram->packet_order,          //НОМЕР ПАКЕТА
                      0x01,                   //TTL
                      0x55555555,       //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,       //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,             //РЕЗЕРВ
                      ram->output_payload);      //ПОЛЕЗНАЯ НАГРУЗКА
}
void packet_Factory_02(WorkTable * ram){
    packetConstructor(ram,
                      '$',              //$
                      0x02,             //ТИП ПАКЕТА
                      ram->MAC,                   //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,   //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,               //ВРЕМЯ
                      ram->my_level,              //УРОВЕНЬ
                      ram->my_session,            //СЕССИЯ
                      0x00,                //СЕАНС
                      0x00,              //РОЛЬ МОДУЛЯ
                      ram->packet_order,          //НОМЕР ПАКЕТА
                      0x01,                   //TTL
                      0x55555555,       //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,       //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,             //РЕЗЕРВ
                      ram->output_payload);      //ПОЛЕЗНАЯ НАГРУЗКА

}
void packet_Factory_03(WorkTable * ram){
    packetConstructor(ram,
                      '$',              //$
                      0x03,             //ТИП ПАКЕТА
                      ram->MAC,                   //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,   //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,               //ВРЕМЯ
                      ram->my_level,              //УРОВЕНЬ
                      ram->my_session,            //СЕССИЯ
                      0x00,                //СЕАНС
                      0x01,              //РОЛЬ МОДУЛЯ
                      ram->packet_order,          //НОМЕР ПАКЕТА
                      0x01,                   //TTL
                      0x55555555,       //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,       //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,             //РЕЗЕРВ
                      ram->output_payload);      //ПОЛЕЗНАЯ НАГРУЗКА
}
void packet_Factory_04(WorkTable * ram){
    packetConstructor(ram,
                      '$',              //$
                      0x04,             //ТИП ПАКЕТА
                      ram->gateway,                   //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,   //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,               //ВРЕМЯ
                      ram->my_level,              //УРОВЕНЬ
                      ram->my_session,            //СЕССИЯ
                      ram->my_seance,                //СЕАНС
                      0x00,              //РОЛЬ МОДУЛЯ
                      ram->packet_order,          //НОМЕР ПАКЕТА
                      0x1e,                   //TTL
                      0xffffffff,       //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,       //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,             //РЕЗЕРВ
                      ram->output_payload);      //ПОЛЕЗНАЯ НАГРУЗКА

}
void packet_Factory_05(WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      '$',                                //$
                      0x05,                               //ТИП ПАКЕТА
                      ram->MAC,                                     //АДРЕС ОТПРАВИТЕЛЯ
                      ram->my_routers[choise],                      //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,                                 //ВРЕМЯ
                      ram->my_level,                                //УРОВЕНЬ
                      ram->my_session,                              //СЕССИЯ
                      ram->my_seance,                               //СЕАНС
                      0x00,                                //РОЛЬ МОДУЛЯ
                      ram->packet_order,                            //НОМЕР ПАКЕТА
                      0x01,                                     //TTL
                      0x55555555,                         //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,                         //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,                               //РЕЗЕРВ
                      ram->output_payload);                        //ПОЛЕЗНАЯ НАГРУЗКА

}
void packet_Factory_06(WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      '$',                                //$
                      0x06,                               //ТИП ПАКЕТА
                      ram->MAC,                                     //АДРЕС ОТПРАВИТЕЛЯ
                      ram->gateway,                                 //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,                                 //ВРЕМЯ
                      ram->my_level,                                //УРОВЕНЬ
                      ram->my_session,                              //СЕССИЯ
                      ram->my_seance,                               //СЕАНС
                      ram->my_role,                                 //РОЛЬ МОДУЛЯ
                      ram->packet_order,                            //НОМЕР ПАКЕТА
                      0x01,                                     //TTL
                      ram->my_routers[choise],                      //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      ram->temporary_prev_address,                  //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,                               //РЕЗЕРВ
                      ram->output_payload);                        //ПОЛЕЗНАЯ НАГРУЗКА

}