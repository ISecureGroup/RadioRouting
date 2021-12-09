#include <stdio.h>
#include "../../core/protocol/protocol.h"

void PrintStatus(STATE stt){
    switch (stt) {
        case 0:printf(" СОН\n");break;
        case 1:printf(" Я УЗЕЛ\n");break;
        case 2:printf(" ВЫБИРАЮ РОУТЕРЫ\n");break;
        case 3:printf(" Я ВЫБРАЛ РОУТЕРЫ\n");break;
        case 4:printf(" ПРИШЛО ПОДТВЕРЖДЕНИЕ\n");break;
        case 5:printf(" НЕ ПРИШЛО ПОДТВЕРЖДЕНИЕ\n");break;
        case 6:printf(" Я НАШЕЛ СВОЙ АДРЕСС ПРИ ОПРОСЕ \n");break;
        case 7:printf(" МОЙ ПАКЕТ UNO УСПЕШНО ДОСТАВЛЕН \n");break;
        case 8:printf(" ПРИШЕЛ ПАКЕТ НА РЕТРАНСЛЯЦИЮ \n");break;
    }
}
void PrintAddress(unsigned long address){
    printf("\t%02lx\t%02lx\t%02lx\t%02lx\n",(address>>24),(address>>16) & 255,(address>>8) & 255,(address) & 255);
}
void ShowRAMTable(WorkTable *ram){

    printf("\n\n|------------------------------ПАМЯТЬ УСТРОЙСТВА 0x%lx--------------------------------\n",ram->MAC);
    printf("|  Список потенциальных роутеров\n");
    int i = 0;
    while(ram->pRouterlist[i].address!=0){
        printf("|  %d\t|",ram->pRouterlist[i].device_counter); PrintAddress(ram->pRouterlist[i].address);
        i++;
    }
    printf("|------------------------------------------------------------------------------------------\n");
    printf("|  Уровень %x	Время 	%x   Статус ->",	ram->my_level, ram->my_time); PrintStatus(ram->Status);
    printf("|  Сессия  %x	Сеанс	%x \n",	ram->my_session,ram->my_seance);
    printf("|------------------------------------------------------------------------------------------\n");
    printf("Данные с датчиков\n");
    i = 0;
    if(ram->many_payload[0]!=0)
        while(ram->many_payload[i]!='#'){
            printf("%02x",(ram->many_payload[i]));
            printf(" ");
            i++;
        }
    printf("\n|------------------------------------------------------------------------------------------\n");
    printf("Кто выбрал меня роутером\n");
    i = 0;
    while(ram->my_end_devices[i]!=0){
        PrintAddress(ram->my_end_devices[i]);
        i++;
    }
    printf("|------------------------------------------------------------------------------------------\n");
    printf("Кто выбрал меня резервным роутером\n");
    i = 0;
    while(ram->i_reserve_router_from[i]!=0){
        PrintAddress(ram->i_reserve_router_from[i]);
        i++;
    }
    printf("|------------------------------------------------------------------------------------------\n");
}
void PrintPacket(Packet exmpl){
    printf("\n\n\n|-----------------------------------Входящий пакет-----------------------------------------\n");
    printf("|  Стартовый байт| %x \t|	Адрес узла отправителя	| ",exmpl._startpacket);		PrintAddress(exmpl._sourceaddres);
    printf("|  Тип пакета    | %x \t|	Адрес узла получателя	| ",exmpl._typepacket);			PrintAddress(exmpl._destinationaddres);
    printf("|----------------|-------------------------------------------------------------------------\n");
    printf("|  Уровень       | %x \t|	Адрес следующего узла	| ",exmpl._level);				PrintAddress(exmpl._nextaddres);
    printf("|  Сессия        | %x \t|	Адрес предыдущего узла	| ",exmpl._session);			PrintAddress(exmpl._prevaddres);
    printf("|  Сеанс         | %x \t|	Время                   |\t%x\n",exmpl._seance,			exmpl._synctime);
    printf("|  Статус узла   | %x \t|	Резервное поле         	|\t%x\n",exmpl._nodestate,		exmpl._reserve);
    printf("|  Номер пакета  | %x \t|	TTL                     |\t%x\n",exmpl._ordernumder,	exmpl._ttl);
    printf("|----------------|------------------Полезная нагрузка--------------------------------------\n");
    int i = 0;
    while(exmpl._payload[i]!='#'){
        printf("%02x",exmpl._payload[i]);
        printf(" ");

        i++;
    }
    printf("\n|--------------------------------Конец полезной нагрузки-----------------------------------");
}



