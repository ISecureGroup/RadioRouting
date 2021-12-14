#include <vcruntime_string.h>
#include "protocol.h"
#include "../../tests/showme.h"
#include "controller/controller.h"
#include "handlers/handlers.h"
#include "factorys/factorys.h"
////////////////////////////////////////////////TOOLKIT////////////////////////////////////////////////////////
unsigned long GetAddress(const unsigned char *stream, int startbyte){
    unsigned long buf = 0;
    return ((((((((buf + stream[startbyte])<<8) + stream[startbyte+1])<<8)+ stream[startbyte+2])<<8)+ stream[startbyte+3]));
}
////////////////////////////////////////////////METHODS////////////////////////////////////////////////////////
int           SetDefault(WorkTable *ram){
    ShowEvent("ВАЛИДАТОР ЗАПУСТИЛ ПРОЦЕСС СБРОСА УСТРОЙСТВА");
    for(int i=0; i < LEN_PAYLOAD; i++)
        ram->many_payload[i] = 0;
    for(int i=0;i<MAX_SUB_ROUTERS;i++)
        ram->i_reserve_router_from[i] = 0;
    for(int i=0;i<MAX_END_DEVICES;i++)
        ram->my_end_devices[i] = 0;
    for(int i=0;i<MAX_POTENTIAL_ROUTER;i++) {
        ram->pRouterlist[i].address = 0;
        ram->pRouterlist[i].device_counter = 0;
    }
    for(int i=0;i<MAX_SUB_ROUTERS;i++)
        ram->my_subrouters[i] = 0;
    ram->my_routers[0]=0;
    ram->my_routers[1]=0;

    ram->Device = 0;
    ram->Status = 0;
    ram->len_of_list = 0;
    ram->my_role = 0;
    ram->my_level = 0;
    ram->my_time = 0;
    ram->my_seance = 0;
    ram->packet_order = 0;
    ram->gateway = 0;
    ram->temporary_prev_address = 0;

    ShowEvent("ПАМЯТЬ ОЧИЩЕНА");
    ShowRAMTable(ram);





}
Packet        ParcerHeader(const unsigned char *stream){

    Packet 				buffer;							//  Temporary storage of parcer packet
    int					len = 0;						//  Parsing...
    unsigned long		adr_buff = 0;

    while(stream[len]!='#')
        len++;
    if(len<HEADER_LEN ||len>LEN_PAYLOAD + HEADER_LEN){
        buffer._typepacket = 0x99;
        return buffer;
    }


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
void          ServiceFieldAdding(WorkTable *ram,Packet pack){
    ram->my_session                      = pack._session;
    ram->my_level                        = pack._level + 1;
    ram->my_seance                       = pack._seance;
    ram->my_time                         = pack._synctime;
}
unsigned char VALIDATOR(WorkTable * ram, Packet pack){

    if(pack._session != ram->my_session )
        SetDefault(ram);

    switch(pack._typepacket)
    {
        case 0x00:	if(ram->Status == SLEEP)                                        {  return pack._typepacket; }   else { return 0x99; }
        case 0x01:	if(ram->Status == START_DEFINING_ROUTERS)                       {  return pack._typepacket; }   else { return 0x99; }
        case 0x02:	if(ram->Status == WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES)   {  return pack._typepacket; }	else { return 0x99; }
        case 0x03:	if(ram->Status == CONFIRM_FROM_POTENTIAL_ROUTER)                {  return pack._typepacket; }	else { return 0x99; }
        case 0x04:	if(ram->Status == READY)                                        {  return pack._typepacket; }	else { return 0x99; }
        case 0x05:	if(ram->Status == RETRANSLATE)                                  {  return pack._typepacket; }	else { return 0x99; }
        case 0x06:	if(ram->Status == RETRANSLATE)                                  {  return pack._typepacket; }	else { return 0x99; }
        case 0x99:  return 0x99;
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
////////////////////////////////////////////////MANAGER////////////////////////////////////////////////////////
void          PacketManager(unsigned char *sens, int RSSI, WorkTable *ram, unsigned char *stream){

    Packet buffer = ParcerHeader(stream);
    //------------Обработчики-----------------
    switch(VALIDATOR(ram, buffer))
    {
        case 0x00:	pl_Handler_00(ram, buffer);	break;
        case 0x01:	pl_Handler_01(ram, buffer);	break;
        case 0x02:	pl_Handler_02(ram, buffer);	break;
        case 0x03:	pl_Handler_03(ram, buffer);	break;
        case 0x04:	pl_Handler_04(ram, buffer);	break;
        case 0x05:	pl_Handler_05(ram, buffer);	break;
        case 0x06:	pl_Handler_06(ram, buffer);	break;
        case 0x99:  ShowEvent("ВАЛИДАТОР ОТБРОСИЛ ПАКЕТ");
    }
    //---------Управляющая логика-------------
    MAIN_CONTROLLER(ram);
    ShowEvent("ПОСЛЕ КОНТРОЛЛЕРА");
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


