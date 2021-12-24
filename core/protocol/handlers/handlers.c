#include "../protocol.h"

////////////////////////////////////////////////HANDLERS///////////////////////////////////////////////////////
void pl_Handler_00(WorkTable * ram, Packet pack, int RSSI){

    for(int i = 0; i<MAX_POTENTIAL_ROUTER; i++)
        if(ram->pRouterlist[i].address == 0 || ram->pRouterlist[i].address == pack._sourceaddres){
            ram->pRouterlist[i].address          = pack._sourceaddres;
            ram->pRouterlist[i].device_counter   = 1;
            ram->pRouterlist[i].rssi             = RSSI;
            ServiceFieldAdding(ram,pack);
            break;
        }
}
void pl_Handler_01(WorkTable * ram, Packet pack){

    unsigned long buffer;
    for(int i = 0;i < pack._plen;i += 4){
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
    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4){
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC){
            ram->Status = ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_04(WorkTable * ram, Packet pack){
    unsigned long 	buffer;
    for(int i=0;i<pack._plen;i+=4){
        buffer = GetAddress(pack._payload,i);
        if(buffer == ram->MAC){
            ram->Status = READY;
        }
    }
    ServiceFieldAdding(ram,pack);
}
void pl_Handler_05(WorkTable * ram, Packet pack){
    for(int iter = 0; iter < LEN_PAYLOAD; iter++){
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
        ram->Status = READY;
    else{
        for(int i=0;i<pack._plen;i+=20)
            if(GetAddress(pack._payload,i) == ram->MAC)
            {
                ram->Status = READY;
            }
    }
}
