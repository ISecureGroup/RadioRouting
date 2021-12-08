#include "protocol.h"
#include "../tests/showme.h"

////////////////////////////////////////////////TOOLKIT////////////////////////////////////////////////////////
Packet        ParcerHeader(const unsigned char *stream){

    Packet 				buffer;								//  Temporary storage of parcer packet
    int					len = 0;						//  Parsing...
    unsigned long		adr_buff = 0;

    while(stream[len]!='#')
        len++;

    buffer._plen                = len - HEADER_LEN-1;
    buffer._startpacket 		= stream[0];
    buffer._typepacket			= stream[1];
    buffer._level				= stream[12];
    buffer._session				= stream[13];
    buffer._seance				= stream[14];
    buffer._nodestate			= stream[15];
    buffer._ordernumder			= stream[16];
    buffer._ttl       			= stream[17];


    buffer._sourceaddres		= (((((((adr_buff + stream[2])<<8) + stream[3])<<8) + stream[4])<<8) + stream[5]); 		adr_buff = 0;
    buffer._destinationaddres	= (((((((adr_buff + stream[6])<<8) + stream[7])<<8) + stream[8])<<8) + stream[9]); 		adr_buff = 0;
    buffer._nextaddres			= (((((((adr_buff + stream[18])<<8) + stream[19])<<8) + stream[20])<<8) + stream[21]);	adr_buff = 0;
    buffer._prevaddres			= (((((((adr_buff + stream[22])<<8) + stream[23])<<8) + stream[24])<<8) + stream[25]);	adr_buff = 0;

    buffer._synctime			=((adr_buff + stream[10])<<8) + stream[11];	adr_buff = 0;
    buffer._reserve				=((adr_buff + stream[26])<<8) + stream[27];	adr_buff = 0;

    for(int iter = 28;iter<=len;iter++)
        buffer._payload[iter-28]= stream[iter];

    return buffer;
}
unsigned long GetAddress(const unsigned char *stream, int startbyte){
    unsigned long buf = 0;
    return ((((((((buf + stream[startbyte])<<8) + stream[startbyte+1])<<8)+ stream[startbyte+2])<<8)+ stream[startbyte+3]));
}
void          ServiceFieldAdding(WorkTable *ram,Packet pack){
    ram->my_session                      = pack._session;
    ram->my_level                        = pack._level + 1;
    ram->my_seance                       = pack._seance;
    ram->my_time                         = pack._synctime;
}
unsigned char PacketValidator(WorkTable * ram, Packet pack){

    switch(pack._typepacket)
    {
        case 0x00:	if(ram->STATUS == SLEEP || pack._session != ram->my_session)    {  return pack._typepacket; }   else { return 0x99; }
        case 0x01:	if(ram->STATUS == LISTENING_MEDIUM)                             {  return pack._typepacket; }   else { return 0x99; }
        case 0x02:	if(ram->STATUS == SELECT_ROUTERS)                               {  return pack._typepacket; }	else { return 0x99; }
        case 0x03:	if(ram->STATUS == ROUTER_WAIT_ELECTION)                         {  return pack._typepacket; }	else { return 0x99; }
        case 0x04:	if(ram->STATUS == READY)                                        {  return pack._typepacket; }	else { return 0x99; }
        case 0x05:	if(ram->STATUS == UNO_MANY || ram->DEVICE == ROUTER)            {  return pack._typepacket; }	else { return 0x99; }
        case 0x06:	if(ram->STATUS == UNO_MANY || ram->DEVICE == ROUTER)            {  return pack._typepacket; }	else { return 0x99; }
    }
    return pack._typepacket;
}
int           getCurrentState(){

}
void          packetConstructor(WorkTable *ram,unsigned char _startpacket,unsigned char _typepacket,unsigned long _sourceaddres,unsigned long _destinationaddres,unsigned short _synctime,unsigned char _level,unsigned char _session,unsigned char _seance,unsigned char _nodestate,unsigned char _ordernumder,unsigned char _ttl,unsigned long _nextaddres,unsigned long _prevaddres,unsigned short _reserve,unsigned char *_payload)
{
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
        if(_payload[i]!='#')
            ram->output_packet._payload[i]      = _payload[i];
        else
            break;
}
////////////////////////////////////////////////CONTROLLER/////////////////////////////////////////////////////
int           MAIN_CONTROLLER(WorkTable * ram){

}
////////////////////////////////////////////////HANDLERS///////////////////////////////////////////////////////
void pl_Handler_00(WorkTable * ram, Packet pack){

    for(int i = 0;i<MAX_POTENTIAL_ROUTER;i++)
        if(ram->pRouterlist[i].address == 0 || ram->pRouterlist[i].address == pack._sourceaddres){
            ram->pRouterlist[i].address          = pack._sourceaddres;
            ram->pRouterlist[i].device_counter   = 1;
            ServiceFieldAdding(ram,pack);
            break;
        }
}
void pl_Handler_01(WorkTable * ram, Packet pack){

    unsigned long buffer;
    for(int i = 0;i < pack._plen;i += 4){
        buffer = 0;
        buffer = GetAddress(pack._payload,i);
        for(int j=0;j<MAX_POTENTIAL_ROUTER;j++){
            if(buffer == ram->pRouterlist[j].address) {
                ram->pRouterlist[j].device_counter++;
                break;
            }
            if(ram->pRouterlist[j].address == 0){
                ram->pRouterlist[j].address         = buffer;
                ram->pRouterlist[j].device_counter  = 1;
                break;
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_02(WorkTable * ram, Packet pack){

    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4){
        buffer = 0;
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC){
            for(int j=0;j<MAX_SUB_ROUTERS;j++){
                if((ram->my_end_devices[j] == 0 || ram->my_end_devices[j] == pack._sourceaddres) && i == 0){
                    ram->my_end_devices[j] = pack._sourceaddres;
                    break;
                }
                if((ram->i_reserve_router_from[j] == 0 || ram->i_reserve_router_from[j] == pack._sourceaddres) && i != 0) {
                    ram->i_reserve_router_from[j] = pack._sourceaddres;
                    break;
                }
            }
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_03(WorkTable * ram, Packet pack){
    unsigned long 	buffer = 0;
    for(int i=0;i<pack._plen;i+=4){
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC){
            ram->STATUS = READY;
        }
        buffer = 0;
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_04(WorkTable * ram, Packet pack){
    unsigned long 	buffer = 0;
    for(int i=0;i<pack._plen;i+=4){
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC){
            ram->STATUS = UNO_MANY;
        }
        buffer = 0;
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_05(WorkTable * ram, Packet pack){
    for(int iter = 0; iter < LEN_PAYLOAD_MANY; iter++){
        if(iter == ram->len_of_list){
            for(int i = 0; i < pack._plen + 1; i++)
                ram->many_payload[iter+i] = pack._payload[i];
            ram->many_payload[iter + pack._plen+1] = '#';
            ram->len_of_list += pack._plen + 1;
            break;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_06(WorkTable * ram, Packet pack){
    if(pack._sourceaddres == ram->MAC)
        ram->STATUS = RETRANSLATE;
    else{
        for(int i=0;i<pack._plen;i+=20)
            if(GetAddress(pack._payload,i) == ram->MAC)
            {
                ram->STATUS = MY_UNO_IS_PACKED;
            }
        }
}
////////////////////////////////////////////////FACTORY////////////////////////////////////////////////////////
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
                      ram->DEVICE,                //РОЛЬ МОДУЛЯ
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
////////////////////////////////////////////////MANAGER////////////////////////////////////////////////////////
void PacketManager(unsigned char *sens, int RSSI, WorkTable *ram, unsigned char *stream){

    Packet buffer = ParcerHeader(stream);
    PrintPacket(buffer);
    //------------Обработчики-----------------
    switch(PacketValidator(ram, buffer))
    {
        case 0x00:	pl_Handler_00(ram, buffer);	break;
        case 0x01:	pl_Handler_01(ram, buffer);	break;
        case 0x02:	pl_Handler_02(ram, buffer);	break;
        case 0x03:	pl_Handler_03(ram, buffer);	break;
        case 0x04:	pl_Handler_04(ram, buffer);	break;
        case 0x05:	pl_Handler_05(ram, buffer);	break;
        case 0x06:	pl_Handler_06(ram, buffer);	break;
        case 0x99:  break;
    }
    //---------Управляющая логика-------------
    MAIN_CONTROLLER(ram);
    //--------------Фабрика-------------------
    switch(getCurrentState())
    {
        case 0:	packet_Factory_00(ram);	break;
        case 1:	packet_Factory_01(ram);	break;
        case 2:	packet_Factory_02(ram);	break;
        case 3:	packet_Factory_03(ram);	break;
        case 4:	packet_Factory_04(ram);	break;
        case 5:	packet_Factory_05(ram);	break;
        case 6:	packet_Factory_06(ram);	break;
    }

    ShowRAMTable(ram);

}


