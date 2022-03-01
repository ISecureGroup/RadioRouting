#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protocol.h"
#include "../../tests/showme.h"
///////////////ОСНОВНЫЕ ПРОЦЕДУРЫ МЕНЕДЖЕРА/////////////////////
Packet          ParsePacket(const unsigned char *stream)
{

    Packet 				buffer;
    int					len = 0;
    unsigned long		adr_buff = 0;

    while(stream[len]!='#')
    {
        len++;
    }

    if(len<HEADER_LEN || len > MAX_LEN_PAYLOAD + HEADER_LEN)
    {
        buffer._typepacket = 0x99;
        return buffer;
    }

    buffer.payload_len          = len - HEADER_LEN - 1;
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

    for(int iter = 28;iter <= len;iter++)
    {
        buffer._payload[iter - 28] = stream[iter];
    }
    return buffer;
}
unsigned char   Validator(WorkTable * ram, Packet pack){

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
void            StatusController(WorkTable * ram){

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
            if(isTimeout(ram, DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES)) {
                if (ram->i_reserve_router_for[0] == 0 && ram->i_main_router_for[0] == 0)
                {
                    ram->Device = END_NODE;
                    ram->Status = READY;
                }
                else
                {
                    ram->Status = SEND_03A;
                    ram->Device = ROUTER;
                }
            }
            break;
            //------------------------------------------------------------
        case READY: break;

            //////////////////////////////////////////////////////////////////////////
    }
}
int             QueueManager(unsigned char *outstream, WorkTable *ram)
{

}
/////////////////////МЕНЕДЖЕР//////////////////////////////////
int PacketManager(unsigned char *sens, int RSSI, WorkTable *ram, unsigned char *instream, unsigned char *outstream, int len)
{

    Packet buffer = ParsePacket(instream);
    //------------Обработчики-----------------
    switch(Validator(ram, buffer))
    {
        case 0x00:  packet_Handler_00(ram, buffer, RSSI);       break;
        case 0x01:  packet_Handler_01(ram, buffer);	            break;
        case 0x02:  packet_Handler_02(ram, buffer);	            break;
        case 0x03:  packet_Handler_03(ram, buffer);	            break;
        case 0x04:  packet_Handler_04(ram, buffer);	            break;
        case 0x05:  packet_Handler_05(ram, buffer);	            break;
        case 0x06:  packet_Handler_06(ram, buffer);	            break;
        case 0x99:  break;
        case 0xff:  break;
    }
    //---------Управляющая логика-------------
    StatusController(ram);
    //--------------Фабрики-------------------
    switch(ram->Status)
    {
        case SEND_00:	packet_Factory_00(outstream,len,ram);	ram->Status = WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;               Queue_up(ram,1,0, ram->output_packet);  break;
        case SEND_01:   packet_Factory_01(outstream,len,ram);	ram->Status = START_DEFINING_ROUTERS;                                   Queue_up(ram,1,0, ram->output_packet);  break;
        case SEND_02:   packet_Factory_02(outstream,len,ram);	ram->Status = WAIT_CONFIRM_FROM_POTENTIAL_ROUTER;                       Queue_up(ram,1,0, ram->output_packet);  break;
        case SEND_03:   packet_Factory_03(outstream,len,ram);   ram->Status = ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;    Queue_up(ram,1,0, ram->output_packet);  break;
        case SEND_03A:  packet_Factory_03(outstream,len,ram);   ram->Status = READY;                                                    Queue_up(ram,1,0, ram->output_packet);  break;
    }

    //---------Менеджер очередей--------------
    return QueueManager(outstream,ram);
}
/////////////////////////ИНСТРУМЕНТЫ////////////////////////////
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
void packetConstructor(WorkTable *ram,
                       unsigned char *outstream,
                       unsigned char _startpacket,
                       unsigned char _typepacket,
                       unsigned long _sourceaddres,
                       unsigned long _destinationaddres,
                       unsigned short _synctime,
                       unsigned char _level,
                       unsigned char _session,
                       unsigned char _seance,
                       unsigned char _nodestate,
                       unsigned char _ordernumder,
                       unsigned char _ttl,
                       unsigned long _nextaddres,
                       unsigned long _prevaddres,
                       unsigned short _reserve,
                       const unsigned char *_payload)
{
    for(int i = 0; i < FULL_PACKET_LENGHT; i++)
        outstream[i] = 0;

    //Служебные поля
    outstream[0] = _startpacket;
    outstream[1] = _typepacket;
    outstream[12] = _level;
    outstream[13] = _session;
    outstream[14] = _seance;
    outstream[15] = _nodestate;
    outstream[16] = _ordernumder;
    outstream[17] = _ttl;
    //Адрес источника
    outstream[2] = _sourceaddres >> 24;
    outstream[3] = (_sourceaddres >> 16) & 255;
    outstream[4] = (_sourceaddres >> 8) & 255;
    outstream[5] = _sourceaddres & 255;
    //Адрес получателя
    outstream[6] = _destinationaddres >> 24;
    outstream[7] = (_destinationaddres >> 16) & 255;
    outstream[8] = (_destinationaddres >> 8) & 255;
    outstream[9] = _destinationaddres & 255;
    //Следущий адрес
    outstream[18] = _nextaddres >> 24;
    outstream[19] = (_nextaddres >> 16) & 255;
    outstream[20] = (_nextaddres >> 8) & 255;
    outstream[21] = _nextaddres & 255;
    //Предыдущий адрес
    outstream[22] = _prevaddres >> 24;
    outstream[23] = (_prevaddres >> 16) & 255;;
    outstream[24] = (_prevaddres >> 8) & 255;
    outstream[25] = _prevaddres & 255;
    //Время
    outstream[10] = _synctime >> 8;
    outstream[11] = _synctime & 255;
    //Резервное поле
    outstream[26] = _reserve >> 8;
    outstream[27] = _reserve & 255;
    //Нагрузка

    for(int i=28;i<FULL_PACKET_LENGHT;i++)
    {
        if(_payload[i]!='#')
            outstream[i] = _payload[i];
        else {
            outstream[i] = '#';
            break;
        }
    }
}
///////////////////////РАБОЧИЕ АЛГОРИТМЫ/////////////////////////
void DefiningRouters(WorkTable *ram)
{

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

    ram->my_routers[MAIN_ROUTER].address = ram->pRouterlist[MAIN_ROUTER].address;
    ram->my_routers[MAIN_ROUTER].accept = 0;
    ram->my_routers[RESERVE_ROUTER].address = ram->pRouterlist[RESERVE_ROUTER].address;
    ram->my_routers[RESERVE_ROUTER].accept = 0;

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

    for(int i=0; i < MAX_LEN_PAYLOAD; i++)
        ram->many_payload[i] = 0;

    for(int i=0; i < MAX_RESERVE_SUBROUTERS; i++)
        ram->i_reserve_router_for[i] = 0;

    for(int i=0; i < MAX_LEN_OF_ROUTER_LIST; i++)
        ram->i_main_router_for[i] = 0;

    for(int i=0; i < MAX_POTENTIAL_ROUTER; i++)
    {
        ram->pRouterlist[i].address = 0;
        ram->pRouterlist[i].device_counter = 0;
    }
    ram->my_routers[0].accept   = 0;
    ram->my_routers[1].accept   = 0;
    ram->my_routers[0].address  = 0;
    ram->my_routers[1].address  = 0;

    ram->Device       = UNDEFINED;
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
void StartInitProtocol(WorkTable * ram)
{
    ram->MAC = 0x69696969; //GetRandomAddress();             // MAC-адрес устройства, в дальнейшем будет случайным
    ram->Status = SLEEP;                                     // Первоначальное состояние устройства
    ram->Device = GATEWAY;
    ram->start_status_time = clock()/CLOCKS_PER_SEC;
}
/////////////////////////ОБРАБОТЧИКИ/////////////////////////////
void packet_Handler_00(WorkTable * ram, Packet pack, int RSSI)
{
    for(int i = 0; i<MAX_POTENTIAL_ROUTER; i++)
    {
        if (ram->pRouterlist[i].address == 0 || ram->pRouterlist[i].address == pack._sourceaddres) {
            ram->pRouterlist[i].address = pack._sourceaddres;
            ram->pRouterlist[i].device_counter = 1;
            ram->pRouterlist[i].rssi = RSSI;

            ServiceFieldAdding(ram, pack);
            break;
        }
    }
}
void packet_Handler_01(WorkTable * ram, Packet pack)
{
    unsigned long buffer;
    for(int i = 0;i < pack.payload_len; i += 4)
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
void packet_Handler_02(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack.payload_len; i+=4)
    {
        buffer = GetAddress(pack._payload,i);

        if(buffer == ram->MAC)
        {
            for(int j=0; j < MAX_LEN_OF_ROUTER_LIST; j++)
            {
                if((ram->i_main_router_for[j] == 0 || ram->i_main_router_for[j] == pack._sourceaddres) && i == 0)
                {
                    ram->i_main_router_for[j] = pack._sourceaddres;
                    break;
                }
                if((ram->i_reserve_router_for[j] == 0 || ram->i_reserve_router_for[j] == pack._sourceaddres) && i != 0)
                {
                    ram->i_reserve_router_for[j] = pack._sourceaddres;
                    break;
                }
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void packet_Handler_03(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack.payload_len; i+=4)
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
void packet_Handler_04(WorkTable * ram, Packet pack)
{
    unsigned long 	buffer;
    for(int i=0;i<pack.payload_len; i+=4)
    {
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC)
        {
            ram->Status = READY;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void packet_Handler_05(WorkTable * ram, Packet pack)
{
    for(int iter = 0; iter < MAX_LEN_PAYLOAD; iter++)
    {
        if(iter == ram->len_of_list)
        {
            for(int i = 0; i < pack.payload_len + 1; i++)
                ram->many_payload[iter+i] = pack._payload[i];
            ram->many_payload[iter + pack.payload_len + 1] = '#';
            ram->len_of_list += pack.payload_len + 1;
            break;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void packet_Handler_06(WorkTable * ram, Packet pack)
{
    if(pack._sourceaddres == ram->MAC)
        ram->Status = READY;
    else
    {
        for(int i=0;i<pack.payload_len; i+=20)
            if(GetAddress(pack._payload,i) == ram->MAC)
            {
                ram->Status = READY;
            }
    }
}
/////////////////////////ФАБРИКИ////////////////////////////////
void packet_Factory_00(unsigned char *outstream,int len,WorkTable * ram){
    for(int i=0; i < MAX_LEN_PAYLOAD; i++)
    {
        ram->output_payload[i] = 0;
    }
    ram->output_payload[0] = '#';
    packetConstructor(ram,
                      outstream,
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
void packet_Factory_01(unsigned char *outstream,int len,WorkTable * ram){

    char buff[4];
    int k = 0;
    for(int i=0; i < MAX_LEN_PAYLOAD; i++)
    {
        ram->output_payload[i] = 0;
    }

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
                      outstream,
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
void packet_Factory_02(unsigned char *outstream,int len,WorkTable * ram){

    char buff[4];
    int k = 0;
    for(int i=0; i < MAX_LEN_PAYLOAD; i++)
    {
        ram->output_payload[i] = 0;
    }

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
                      outstream,
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
void packet_Factory_03(unsigned char *outstream,int len,WorkTable * ram){

    char buff[4];
    int k = 0;

    for(int i = 0; i < MAX_LEN_PAYLOAD; i++)
        ram->output_payload[i] = 0;

    for(int i=0; i < MAX_DEVICES_FOR_WHICH_IM_MAIN_ROUTER; i++)
    {
        if(ram->i_main_router_for[i] != 0)
        {
            GetAddressChar(buff, ram->i_main_router_for[i]);
            for (int j = 0; j < 4; j++) {
                ram->output_payload[k] = buff[j];
                k++;
            }
        }
    }
    for(int i = 0;i < MAX_DEVICES_FOR_WHICH_IM_RESERVE_ROUTER; i++)
    {
        if(ram->i_reserve_router_for[i] != 0) {
            GetAddressChar(buff, ram->i_reserve_router_for[i]);
            for (int j = 0; j < 4; j++) {
                ram->output_payload[k] = buff[j];
                k++;
            }
        }
    }
    ram->output_payload[k] = '#';
    packetConstructor(ram,
                      outstream,
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
void packet_Factory_04(unsigned char *outstream,int len,WorkTable * ram){
    packetConstructor(ram,
                      outstream,
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
void packet_Factory_05(unsigned char *outstream,int len,WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      outstream,
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
void packet_Factory_06(unsigned char *outstream,int len,WorkTable * ram){
    int choise = MAIN_ROUTER;
    packetConstructor(ram,
                      outstream,
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


