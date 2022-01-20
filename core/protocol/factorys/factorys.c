#include "../protocol.h"

///////////////////////////////////////////////////////////////////////////////////////////

void packetConstructor(WorkTable *ram,unsigned char _startpacket,unsigned char _typepacket,unsigned long _sourceaddres,unsigned long _destinationaddres,unsigned short _synctime,unsigned char _level,unsigned char _session,unsigned char _seance,unsigned char _nodestate,unsigned char _ordernumder,unsigned char _ttl,unsigned long _nextaddres,unsigned long _prevaddres,unsigned short _reserve,const unsigned char *_payload)
{
    for(int i=0;i<LEN_PAYLOAD; i++)
        ram->output_packet._payload[i] = 0;

    ram->output_packet._startpacket         = _startpacket;
    ram->output_packet._typepacket          = _typepacket;
    ram->output_packet._sourceaddres        = _sourceaddres;
    ram->output_packet._destinationaddres   = _destinationaddres;
    ram->output_packet._synctime            = _synctime;
    ram->output_packet._session             = _session;
    ram->output_packet._level               = _level;
    ram->output_packet._seance              = _seance;
    ram->output_packet._nodestate           = _nodestate;
    ram->output_packet._ordernumder         = _ordernumder;
    ram->output_packet._ttl                 = _ttl;
    ram->output_packet._nextaddres          = _nextaddres;
    ram->output_packet._prevaddres          = _prevaddres;
    ram->output_packet._reserve             = _reserve;

    for(int i=0;i<100;i++)
    {
        if(_payload[i]!='#')
            ram->output_packet._payload[i] = _payload[i];
        else {
            ram->output_packet._payload[i] = '#';
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_00(WorkTable * ram){
    packetConstructor(ram,
                      '$',                      //$
                      0x00,                     //ТИП ПАКЕТА
                      ram->MAC,                 //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,               //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,             //ВРЕМЯ
                      ram->my_level,            //УРОВЕНЬ
                      ram->my_session,          //СЕССИЯ
                      0x00,                     //СЕАНС
                      ram->Device,              //РОЛЬ МОДУЛЯ
                      0x00,                     //НОМЕР ПАКЕТА
                      0x01,                     //TTL
                      0x55555555,               //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,               //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,                   //РЕЗЕРВ
                      ram->output_payload);     //ПОЛЕЗНАЯ НАГРУЗКА
}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_01(WorkTable * ram){

    char buff[4];
    int k = 0;
    for(int i=0;i<LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;

    for(int i=0;i<MAX_POTENTIAL_ROUTER; i++){
        GetAddressChar(buff, ram->pRouterlist[i].address);
        if(ram->pRouterlist[i].address == 0) {
            ram->output_payload[k] = '#';
            break;
        }
        for(int j=0;j<4;j++) {
            ram->output_payload[k] = buff[j];
            k++;
        }
    }
    packetConstructor(ram,
                      '$',                  //$
                      0x01,                 //ТИП ПАКЕТА
                      ram->MAC,             //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,           //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,         //ВРЕМЯ
                      ram->my_level,        //УРОВЕНЬ
                      ram->my_session,      //СЕССИЯ
                      0x00,                 //СЕАНС
                      0x00,                 //РОЛЬ МОДУЛЯ
                      ram->packet_order,    //НОМЕР ПАКЕТА
                      0x01,                 //TTL
                      0x55555555,           //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,           //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,               //РЕЗЕРВ
                      ram->output_payload); //ПОЛЕЗНАЯ НАГРУЗКА
}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_02(WorkTable * ram){

    char buff[4];
    int k = 0;
    for(int i=0;i<LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;

    for(int i=0;i<2; i++)
    {
        GetAddressChar(buff, ram->my_routers[i].address);
        for(int j=0;j<4;j++) {
            ram->output_payload[k] = buff[j];
            k++;
        }
        if(i == 1) {
            ram->output_payload[k] = '#';
            break;
        }
    }
    packetConstructor(ram,
                      '$',                  //$
                      0x02,                 //ТИП ПАКЕТА
                      ram->MAC,             //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,           //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,         //ВРЕМЯ
                      ram->my_level,        //УРОВЕНЬ
                      ram->my_session,      //СЕССИЯ
                      0x00,                 //СЕАНС
                      0x00,                 //РОЛЬ МОДУЛЯ
                      ram->packet_order,    //НОМЕР ПАКЕТА
                      0x01,                 //TTL
                      0x55555555,           //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,           //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,               //РЕЗЕРВ
                      ram->output_payload); //ПОЛЕЗНАЯ НАГРУЗКА

}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_03(WorkTable * ram){

    char buff[4];
    int k = 0;

    for(int i = 0;i < LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;

    for(int i=0;i < MAX_MAIN_SUBROUTERS; i++)
    {
        GetAddressChar(buff, ram->i_main_router_from[i]);
        for(int j = 0;j<4;j++) {
            ram->output_payload[k] = buff[j];
            k++;
        }
    }
    for(int i = 0;i < MAX_RESERVE_SUBROUTERS; i++)
    {
        GetAddressChar(buff, ram->i_main_router_from[i]);
        for(int j = 0;j < 4;j++) {
            ram->output_payload[k] = buff[j];
            k++;
        }
        if(i = MAX_RESERVE_SUBROUTERS-1)
        {
            ram->output_payload[k] = '#';
            break;
        }
    }
    packetConstructor(ram,
                      '$',                  //$
                      0x03,                 //ТИП ПАКЕТА
                      ram->MAC,             //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,           //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,         //ВРЕМЯ
                      ram->my_level,        //УРОВЕНЬ
                      ram->my_session,      //СЕССИЯ
                      0x00,                 //СЕАНС
                      0x01,                 //РОЛЬ МОДУЛЯ
                      ram->packet_order,    //НОМЕР ПАКЕТА
                      0x01,                 //TTL
                      0x55555555,           //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,           //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,               //РЕЗЕРВ
                      ram->output_payload); //ПОЛЕЗНАЯ НАГРУЗКА
}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_04(WorkTable * ram){
    packetConstructor(ram,
                      '$',                  //$
                      0x04,                 //ТИП ПАКЕТА
                      ram->gateway,         //АДРЕС ОТПРАВИТЕЛЯ
                      0x00000000,           //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,         //ВРЕМЯ
                      ram->my_level,        //УРОВЕНЬ
                      ram->my_session,      //СЕССИЯ
                      ram->my_seance,       //СЕАНС
                      0x00,                 //РОЛЬ МОДУЛЯ
                      ram->packet_order,    //НОМЕР ПАКЕТА
                      0x1e,                 //TTL
                      0xffffffff,           //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,           //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,               //РЕЗЕРВ
                      ram->output_payload); //ПОЛЕЗНАЯ НАГРУЗКА

}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_05(WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      '$',                                //$
                      0x05,                               //ТИП ПАКЕТА
                      ram->MAC,                           //АДРЕС ОТПРАВИТЕЛЯ
                      ram->my_routers[choise].address,    //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,                       //ВРЕМЯ
                      ram->my_level,                      //УРОВЕНЬ
                      ram->my_session,                    //СЕССИЯ
                      ram->my_seance,                     //СЕАНС
                      0x00,                               //РОЛЬ МОДУЛЯ
                      ram->packet_order,                  //НОМЕР ПАКЕТА
                      0x01,                               //TTL
                      0x55555555,                         //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      0x00000000,                         //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,                             //РЕЗЕРВ
                      ram->output_payload);               //ПОЛЕЗНАЯ НАГРУЗКА

}

///////////////////////////////////////////////////////////////////////////////////////////

void packet_Factory_06(WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      '$',                                //$
                      0x06,                               //ТИП ПАКЕТА
                      ram->MAC,                           //АДРЕС ОТПРАВИТЕЛЯ
                      ram->gateway,                       //АДРЕС ПОЛУЧАТЕЛЯ
                      ram->my_time,                       //ВРЕМЯ
                      ram->my_level,                      //УРОВЕНЬ
                      ram->my_session,                    //СЕССИЯ
                      ram->my_seance,                     //СЕАНС
                      ram->my_role,                       //РОЛЬ МОДУЛЯ
                      ram->packet_order,                  //НОМЕР ПАКЕТА
                      0x01,                               //TTL
                      ram->my_routers[choise].address,    //АДРЕС СЛЕДУЮЩЕГО УЗЛА
                      ram->temporary_prev_address,        //АДРЕС ПРЕДЫДУЩЕГО УЗЛА
                      0x5555,                             //РЕЗЕРВ
                      ram->output_payload);               //ПОЛЕЗНАЯ НАГРУЗКА

}

///////////////////////////////////////////////////////////////////////////////////////////