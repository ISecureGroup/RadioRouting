#include <time.h>
#include <stdio.h>
#include "../protocol.h"

int isTimeout(WorkTable *ram, unsigned int time) {

    if(ram->actual_status_time_duration > time){
        return 1;
        ram->start_status_time = ram->actual_status_time_duration;
    }
}
void CheckSession(WorkTable *ram) {

}
int MAIN_CONTROLLER(WorkTable * ram){

    ram->actual_status_time_duration = clock()/CLOCKS_PER_SEC - ram->start_status_time;

    switch (ram->Status) {
        //------------------------------------------------------------ 0
        case SLEEP:
            if(isTimeout(ram, 10))
                ram->Status = START_DEFINING_ROUTERS;
            break;
        //------------------------------------------------------------ 1
        case START_DEFINING_ROUTERS:
            if(isTimeout(ram, 10))
                ram->Status = ROUTER_IS_DEFINED;
            break;
        //------------------------------------------------------------ 2
        case ROUTER_IS_DEFINED:
            if(isTimeout(ram, 10))
                ram->Status = CONFIRM_FROM_POTENTIAL_ROUTER;
            break;
        //------------------------------------------------------------ 3
        case CONFIRM_FROM_POTENTIAL_ROUTER:
            if(isTimeout(ram, 10))
                ram->Status = ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS;
            break;
        //------------------------------------------------------------ 4
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:
            if(isTimeout(ram, 10))
                ram->Status = WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;
            break;
        //------------------------------------------------------------ 5
        case WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, 10))
                ram->Status = ANNOUNCEMENT_ROUTER_STATUS;
            break;
        //------------------------------------------------------------ 6
        case ANNOUNCEMENT_ROUTER_STATUS:
            if(isTimeout(ram, 10))
                ram->Status = ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;
            break;
            //------------------------------------------------------------
        case ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            if(isTimeout(ram, 10))
                ram->Status = READY;
            break;
            //------------------------------------------------------------
        case READY:                                                                                                            break;
        case RETRANSLATE:                                                                                                      break;
        case MY_UNO_IS_PACKED:                                                                                                 break;
    }
}

