#include "../protocol.h"

void SetStatus(STATE Stt) {

}
void CheckSession(WorkTable *ram) {

}


int MAIN_CONTROLLER(WorkTable * ram){

    switch (ram->Status) {
        case SLEEP:
            SetStatus(READY_DEFINE_ROUTER);
            SetDefault();
            break;
        case READY_DEFINE_ROUTER:                                                                                              break;
        case WAITING_NEIGHBORS:                                                                                                break;
        case ROUTER_IS_DEFINED:                                                                                                break;
        case CONFIRM_FROM_POTENTIAL_ROUTER:                                                                                    break;
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:                                                                             break;
        case WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:                                                                       break;
        case ANNOUNCEMENT_ROUTER_STATUS:                                                                                       break;
        case ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES:                                                            break;
        case READY:                                                                                                            break;
        case RETRANSLATE:
        case MY_UNO_IS_PACKED:break;
    }
}

