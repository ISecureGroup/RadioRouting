#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protocol.h"
#include "../../tests/showme.h"
///////////////ОСНОВНЫЕ ПРОЦЕДУРЫ МЕНЕДЖЕРА/////////////////////
Packet          ParcerHeader(const unsigned char *stream)
{

    Packet 				buffer;							//ВРЕМЕННЫЙ БУФЕР ДЛЯ ПАКЕТА
    int					len = 0;
    unsigned long		adr_buff = 0;

    while(stream[len]!='#')                             //ВЫЧИСЛЕНИЕ ПОЛНОЙ ДЛИННЫ ПАКЕТА МИНУС #
        len++;

    if(len<HEADER_LEN ||len>LEN_PAYLOAD + HEADER_LEN)
    {
        buffer._typepacket = 0x99;
        return buffer;
    }

    buffer._plen          = len - HEADER_LEN-1;
    buffer._startpacket   = stream[0];
    buffer._typepacket    = stream[1];
    buffer._level         = stream[12];
    buffer._session       = stream[13];
    buffer._seance        = stream[14];
    buffer._nodestate     = stream[15];
    buffer._ordernumder   = stream[16];
    buffer._ttl           = stream[17];


    buffer._sourceaddres        = (((((((adr_buff + stream[2])<<8) + stream[3])<<8) + stream[4])<<8) + stream[5]); 		adr_buff = 0;
    buffer._destinationaddres   = (((((((adr_buff + stream[6])<<8) + stream[7])<<8) + stream[8])<<8) + stream[9]); 		adr_buff = 0;
    buffer._nextaddres          = (((((((adr_buff + stream[18])<<8) + stream[19])<<8) + stream[20])<<8) + stream[21]);	adr_buff = 0;
    buffer._prevaddres          = (((((((adr_buff + stream[22])<<8) + stream[23])<<8) + stream[24])<<8) + stream[25]);	adr_buff = 0;

    buffer._synctime            =((adr_buff + stream[10])<<8) + stream[11];	adr_buff = 0;
    buffer._reserve             =((adr_buff + stream[26])<<8) + stream[27];   adr_buff = 0;

    for(int iter = 28;iter<=len;iter++)
        buffer._payload[iter-28] = stream[iter];

    return buffer;
}
unsigned char   VALIDATOR(WorkTable * ram, Packet pack){

    if(pack._startpacket != '$')
        return 0x99;

    if(pack._session != ram->my_session)
    {
        if (pack._typepacket == 0x00)
            SetDefault(ram);
        else return 0x99;
    }
    switch(pack._typepacket)
    {
        case 0x00:	if(ram->Status == SLEEP)                                                                                                                {  return pack._typepacket; }     else { return 0x99; }
        case 0x01:	if(ram->Status == START_DEFINING_ROUTERS)                                                                                               {  return pack._typepacket; }     else { return 0x99; }
        case 0x02:	if(ram->Status == WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES || ram->Status == ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES)   {  return pack._typepacket; }     else { return 0x99; }
        case 0x03:	if(ram->Status == WAIT_CONFIRM_FROM_POTENTIAL_ROUTER)                                                                                   {  return pack._typepacket; }     else { return 0x99; }
        case 0x99:  return 0x99;
    }
    return pack._typepacket;
}
void            MAIN_CONTROLLER(WorkTable * ram){

    ram->delta_time = clock()/CLOCKS_PER_SEC - ram->start_status_time;

    switch (ram->Status)
    {
        //------------------------------------------------------------
        case SEND_00: break;
            //------------------------------------------------------------
        case SLEEP:
            if(isTimeout(ram, DELAY_OF_SLEEP) && ram->pRouterlist[0].address != 0)
            {
                ram->Status = SEND_01;
            }
            break;
            //------------------------------------------------------------
        case SEND_01: break;
            //------------------------------------------------------------
        case START_DEFINING_ROUTERS:
            if(isTimeout(ram, DELAY_OF_START_DEFINING_ROUTERS))
            {
                DefiningRouters(ram);
                ram->Status = SEND_02;
            }
            break;
            //------------------------------------------------------------
        case SEND_02: break;
            //------------------------------------------------------------
        case WAIT_CONFIRM_FROM_POTENTIAL_ROUTER:
            if(isTimeout(ram, DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER) || (ram->my_routers[0].accept == ram->my_routers[1].accept == 1))
            {
                if (ram->my_routers[0].accept == 1 || ram->my_routers[1].accept == 1)
                {
                    ram->Status = ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS;
                }
                else
                {
                    ram->Status = SLEEP;
                }
            }
            break;
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:
            if(isTimeout(ram, DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS))
                ram->Status = SEND_00;
            break;
            //------------------------------------------------------------
        case WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES))
                ram->Status = SEND_03;
            break;
            //------------------------------------------------------------
        case SEND_03: break;
            //------------------------------------------------------------
        case ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES))
                ram->Status = SEND_03A;
            break;
            //------------------------------------------------------------
        case READY: break;

            //////////////////////////////////////////////////////////////////////////
    }
}
void            QUEUE_MANAGER(WorkTable *ram)
{
}
/////////////////////МЕНЕДЖЕР//////////////////////////////////
void PacketManager(unsigned char *sens, int RSSI, WorkTable *ram, unsigned char *stream)
{

    Packet buffer = ParcerHeader(stream);
    PrintPacketLine("->",buffer);
    //------------Обработчики-----------------
    switch(VALIDATOR(ram, buffer))
    {
        case 0x00:	pl_Handler_00(ram, buffer, RSSI);       break;
        case 0x01:	pl_Handler_01(ram, buffer);	            break;
        case 0x02:	pl_Handler_02(ram, buffer);	            break;
        case 0x03:	pl_Handler_03(ram, buffer);	            break;
        case 0x04:	pl_Handler_04(ram, buffer);	            break;
        case 0x05:	pl_Handler_05(ram, buffer);	            break;
        case 0x06:	pl_Handler_06(ram, buffer);	            break;
        case 0x99:                                          break;
        case 0xff:  break;
    }
    //---------Управляющая логика-------------
    MAIN_CONTROLLER(ram);
    //--------------Фабрики-------------------
    switch(ram->Status)
    {
        case SEND_00:	packet_Factory_00(ram);	ram->Status = WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;               break;
        case SEND_01:   packet_Factory_01(ram);	ram->Status = START_DEFINING_ROUTERS;                                   break;
        case SEND_02:   packet_Factory_02(ram);	ram->Status = WAIT_CONFIRM_FROM_POTENTIAL_ROUTER;                       break;
        case SEND_03:   packet_Factory_03(ram); ram->Status = ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;    break;
        case SEND_03A:  packet_Factory_03(ram); ram->Status = READY;                                                    break;
    }
    //---------Менеджер очередей--------------
    QUEUE_MANAGER(ram);
}
/////////////////////////ИНСТРУМЕНТЫ/////////////////////////////
unsigned long GetRandomAddress()
{
    unsigned long address;
    unsigned char buff[4];
    while(1){
        srand(time(NULL));
        address = rand();
        GetAddressChar(buff,address);
        if(buff[0] !='#' && buff[1] !='#' && buff[2] !='#' && buff[3] !='#')
            return address;
    }
}
unsigned long GetAddress(const unsigned char *stream, int startbyte)
{
    unsigned long buf = 0;
    return ((((((((buf + stream[startbyte])<<8) + stream[startbyte+1])<<8)+ stream[startbyte+2])<<8)+ stream[startbyte+3]));
}
void GetAddressChar(char* buff, unsigned long stream)
{
    buff[0] = (char)(stream>>24);
    buff[1] = (char)((stream>>16) & 255);
    buff[2] = (char)((stream>>8)  & 255);
    buff[3] = (char)((stream)     & 255);
}
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
    PrintPacketLine("<-", ram->output_packet);
}
///////////////////////РАБОЧИЕ АЛГОРИТМЫ/////////////////////////
void DefiningRouters(WorkTable *ram) {

    RouteUnit buffer;
    for (int i = 0; i < MAX_POTENTIAL_ROUTER-1; i++)
    {
        for (int j = MAX_POTENTIAL_ROUTER-1; j > i; j--)
        {
            if (ram->pRouterlist[j].device_counter > ram->pRouterlist[j-1].device_counter)
            {
                buffer = ram->pRouterlist[j - 1];
                ram->pRouterlist[j - 1] = ram->pRouterlist[j];
                ram->pRouterlist[j]     = buffer;
            }
            if(ram->pRouterlist[j].device_counter == ram->pRouterlist[j-1].device_counter)
            {
                if(ram->pRouterlist[j].rssi > ram->pRouterlist[j-1].rssi)
                {
                    buffer = ram->pRouterlist[j - 1];
                    ram->pRouterlist[j - 1] = ram->pRouterlist[j];
                    ram->pRouterlist[j]     = buffer;
                }

            }
        }
    }

    ram->my_routers[0].address = ram->pRouterlist[0].address;
    ram->my_routers[0].accept = 0;
    ram->my_routers[1].address = ram->pRouterlist[1].address;
    ram->my_routers[1].accept = 0;

}
int  isTimeout(WorkTable *ram, unsigned int delay) {

    if(ram->delta_time > delay)
    {
        ram->start_status_time = ram->start_status_time + ram->delta_time;
        return 1;
    }
    return 0;
}
void SetDefault(WorkTable *ram)
{

    for(int i=0; i < LEN_PAYLOAD; i++)
        ram->many_payload[i] = 0;

    for(int i=0; i < MAX_RESERVE_SUBROUTERS; i++)
        ram->i_reserve_router_from[i] = 0;

    for(int i=0; i < MAX_MAIN_SUBROUTERS; i++)
        ram->i_main_router_from[i] = 0;

    for(int i=0; i < MAX_POTENTIAL_ROUTER; i++)
    {
        ram->pRouterlist[i].address = 0;
        ram->pRouterlist[i].device_counter = 0;
    }
    for(int i=0; i < MAX_RESERVE_SUBROUTERS; i++)
        ram->my_subrouters[i] = 0;

    ram->my_routers[0].accept   = 0;
    ram->my_routers[1].accept   = 0;
    ram->my_routers[0].address  = 0;
    ram->my_routers[1].address  = 0;

    ram->Device       = END_NODE;
    ram->Status       = SLEEP;
    ram->len_of_list  = 0;
    ram->my_role      = 0;
    ram->my_level     = 0;
    ram->my_time      = 0;
    ram->my_seance    = 0;
    ram->packet_order = 0;
    ram->gateway      = 0;
    ram->temporary_prev_address = 0;
}
void Queue_up(WorkTable *ram, unsigned int repeat, unsigned int time_to_send, Packet exmpl){

    for(int i = 0; i < 4; i++)
    {
        ram->QUEUE[i].isDelivered      = ram->QUEUE[i+1].isDelivered;
        ram->QUEUE[i].repeat           = ram->QUEUE[i+1].repeat;
        ram->QUEUE[i].time_to_send     = ram->QUEUE[i+1].time_to_send;
        ram->QUEUE[i].q_packet         = ram->QUEUE[i+1].q_packet;
    }
    ram->QUEUE[4].isDelivered      = 0;
    ram->QUEUE[4].repeat           = repeat;
    ram->QUEUE[4].time_to_send     = time_to_send;
    ram->QUEUE[4].q_packet         = exmpl;
}
void ServiceFieldAdding(WorkTable *ram,Packet pack)
{
    ram->my_session     = pack._session;
    ram->my_level       = pack._level + 1;
    ram->my_seance      = pack._seance;
    ram->my_time        = pack._synctime;
}
/////////////////////////ОБРАБОТЧИКИ/////////////////////////////
void pl_Handler_00(WorkTable * ram, Packet pack, int RSSI)
{
    for(int i = 0; i<MAX_POTENTIAL_ROUTER; i++)
        if(ram->pRouterlist[i].address == 0 || ram->pRouterlist[i].address == pack._sourceaddres)
        {
            ram->pRouterlist[i].address          = pack._sourceaddres;
            ram->pRouterlist[i].device_counter   = 1;
            ram->pRouterlist[i].rssi             = RSSI;

            ServiceFieldAdding(ram,pack);
            break;
        }
}
void pl_Handler_01(WorkTable * ram, Packet pack)
{
    unsigned long buffer;
    for(int i = 0;i < pack._plen;i += 4)
    {
        buffer = GetAddress(pack._payload,i);
        for(int j=0;j<MAX_POTENTIAL_ROUTER;j++)
        {
            if(buffer == ram->pRouterlist[j].address)
            {
                ram->pRouterlist[j].device_counter++;
                break;
            }
            if(ram->pRouterlist[j].address == 0)
            {
                ram->pRouterlist[j].address         = buffer;
                ram->pRouterlist[j].device_counter  = 1;
                break;
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_02(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4)
    {
        buffer = GetAddress(pack._payload,i);

        if(buffer == ram->MAC)
        {
            for(int j=0;j<MAX_MAIN_SUBROUTERS;j++)
            {
                if((ram->i_main_router_from[j] == 0 || ram->i_main_router_from[j] == pack._sourceaddres) && i == 0)
                {
                    ram->i_main_router_from[j] = pack._sourceaddres;
                    break;
                }
                if((ram->i_reserve_router_from[j] == 0 || ram->i_reserve_router_from[j] == pack._sourceaddres) && i != 0)
                {
                    ram->i_reserve_router_from[j] = pack._sourceaddres;
                    break;
                }
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_03(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4)
    {
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC)
        {
            for(int j=0;j<2;j++)
            {
                if(ram->my_routers[j].address == pack._sourceaddres)
                {
                    ram->my_routers[j].accept = 1;
                }
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_04(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4)
    {
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC)
        {
            ram->Status = READY;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_05(WorkTable * ram, Packet pack)
{
    for(int iter = 0; iter < LEN_PAYLOAD; iter++)
    {
        if(iter == ram->len_of_list)
        {
            for(int i = 0; i < pack._plen + 1; i++)
                ram->many_payload[iter+i] = pack._payload[i];
            ram->many_payload[iter + pack._plen+1] = '#';
            ram->len_of_list += pack._plen + 1;
            break;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_06(WorkTable * ram, Packet pack)
{
    if(pack._sourceaddres == ram->MAC)
        ram->Status = READY;
    else
    {
        for(int i=0;i<pack._plen;i+=20)
            if(GetAddress(pack._payload,i) == ram->MAC)
            {
                ram->Status = READY;
            }
    }
}
/////////////////////////ФАБРИКИ////////////////////////////////
void packet_Factory_00(WorkTable * ram){
    for(int i=0;i<LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;
    ram->output_payload[0] = '#';
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
void packet_Factory_03(WorkTable * ram){

    char buff[4];
    int k = 0;

    for(int i = 0;i < LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;

    for(int i=0;i < MAX_MAIN_SUBROUTERS; i++)
    {
        if(ram->i_main_router_from[i] != 0)
        {
            GetAddressChar(buff, ram->i_main_router_from[i]);
            for (int j = 0; j < 4; j++) {
                ram->output_payload[k] = buff[j];
                k++;
            }
        }
    }
    for(int i = 0;i < MAX_RESERVE_SUBROUTERS; i++)
    {
        if(ram->i_reserve_router_from[i] != 0) {
            GetAddressChar(buff, ram->i_reserve_router_from[i]);
            for (int j = 0; j < 4; j++) {
                ram->output_payload[k] = buff[j];
                k++;
            }
        }
    }
    ram->output_payload[k] = '#';
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


