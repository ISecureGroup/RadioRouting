#include <time.h>
#include <stdio.h>
#include "../protocol.h"

int isTimeout(WorkTable *ram, unsigned int delay) {

    if(ram->delta_time > delay){
        ram->start_status_time = ram->start_status_time + ram->delta_time;
        return 1;
    }
    return 0;
}

int MAIN_CONTROLLER(WorkTable * ram){

    ram->delta_time = clock() / CLOCKS_PER_SEC - ram->start_status_time;

    switch (ram->Status) {
        //------------------------------------------------------------ 0
        case SLEEP:
            if(isTimeout(ram, DELAY_OF_SLEEP) && ram->pRouterlist[0].address != 0) {
                ram->Status = START_DEFINING_ROUTERS;
            }
            break;
        //------------------------------------------------------------ 1
        case START_DEFINING_ROUTERS:
            if(isTimeout(ram, DELAY_OF_START_DEFINING_ROUTERS))
                ram->Status = ROUTER_IS_DEFINED;
            break;
        //------------------------------------------------------------ 2
        case ROUTER_IS_DEFINED:
            if(isTimeout(ram, DELAY_OF_ROUTER_IS_DEFINED))
                ram->Status = CONFIRM_FROM_POTENTIAL_ROUTER;
            break;
        //------------------------------------------------------------ 3
        case CONFIRM_FROM_POTENTIAL_ROUTER:
            if(isTimeout(ram, DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER))
                ram->Status = ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS;
            break;
        //------------------------------------------------------------ 4
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:
            if(isTimeout(ram, DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS))
                ram->Status = WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;
            break;
        //------------------------------------------------------------ 5
        case WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES))
                ram->Status = ANNOUNCEMENT_ROUTER_STATUS;
            break;
        //------------------------------------------------------------ 6
        case ANNOUNCEMENT_ROUTER_STATUS:
            if(isTimeout(ram, DELAY_OF_ANNOUNCEMENT_ROUTER_STATUS))
                ram->Status = ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;
            break;
            //------------------------------------------------------------
        case ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES))
                ram->Status = READY;
            break;
            //------------------------------------------------------------
        case READY:                                                                                                            break;
        case RETRANSLATE:                                                                                                      break;
        case MY_UNO_IS_PACKED:                                                                                                 break;
    }
}

