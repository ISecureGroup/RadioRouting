#include "protocol.hh"
#include "Arduino.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////

int isTimeout(WorkTable *ram, unsigned int delay) {

    if(ram->delta_time > delay)
    {
        ram->start_status_time = ram->start_status_time + ram->delta_time;
        return 1;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////

void MAIN_CONTROLLER(WorkTable * ram){

    ram->delta_time = millis() - ram->start_status_time;

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
        //////////////////////////////////////////////////////////////////////////
        case ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS:
            if(isTimeout(ram, DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS))
                ram->Status = WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES;
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
                ram->Status = READY;
            break;
            //------------------------------------------------------------
        case READY: break;

    }
}



