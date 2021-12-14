#include "../protocol.h"

void SetStatus(WorkTable *ram,STATE Stt) {
    ram->Status = Stt;
}
void CheckSession(WorkTable *ram) {

}


int MAIN_CONTROLLER(WorkTable * ram){

    switch (ram->Status) {
        //------------------------------------------------------------ 0
        case SLEEP:
            SetStatus(ram,START_DEFINING_ROUTERS);
            break;
        //------------------------------------------------------------ 1
        case START_DEFINING_ROUTERS:
            SetStatus(ram,ROUTER_IS_DEFINED);
            break;
        //------------------------------------------------------------ 2
        case ROUTER_IS_DEFINED:
            SetStatus(ram,CONFIRM_FROM_POTENTIAL_ROUTER);
            break;
        //------------------------------------------------------------ 3
        case CONFIRM_FROM_POTENTIAL_ROUTER:
            SetStatus(ram,ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS);
            break;
        //------------------------------------------------------------ 4
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:
            SetStatus(ram,WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES);
            break;
        //------------------------------------------------------------ 5
        case WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            SetStatus(ram,ANNOUNCEMENT_ROUTER_STATUS);
            break;
        //------------------------------------------------------------ 6
        case ANNOUNCEMENT_ROUTER_STATUS:
            SetStatus(ram,ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES);
            break;
            //------------------------------------------------------------
        case ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:
            SetStatus(ram,READY);
            break;
            //------------------------------------------------------------
        case READY:                                                                                                            break;
        case RETRANSLATE:
        case MY_UNO_IS_PACKED:                                                                                                 break;
    }
}

