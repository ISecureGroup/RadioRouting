#include <stdio.h>
#include <stdlib.h>
#include "../../core/protocol/protocol.h"

void ShowEvent(char *s)
{
    printf("\n|---------------------------------%s---------------------------------\n",s);
}
void PrintStatus(STATE stt)
{
    switch (stt) {
        //////////////////ПЕРВЫЙ ЭТАП//////////////////////////////
        case 0:printf(" ОТПРАВЛЯЮ 00\n");                               break;
        case 1:printf(" СОН\n");                                        break;
        case 2:printf(" ОТПРАВЛЯЮ 01\n");                               break;
        case 3:printf(" ОПРЕДЕЛЯЮ РОУТЕРЫ\n");                          break;
        case 4:printf(" ОТПРАВЛЯЮ 02\n");                               break;
        case 5:printf(" ЖДЕМ ПОДТВЕРЖДЕНИЯ ОТ РОУТЕРА\n");              break;
            ////////////////// ВТОРОЙ ЭТАП/////////////////////////////
        case 6:printf(" ОБЪЯВЛЯЮ СЕБЯ РОУТЕРОМ\n");                     break;
        case 7:printf(" ОЖИДАЮ ПОДТВЕРЖДЕНИЯ ОТ УСТРОЙСТВ\n");          break;
        case 8:printf(" ОТПРАВЛЯЮ 03 \n");                              break;
        case 9:printf(" ЕЩЕ РАЗ ОЖИДАЮ ПОДТВЕРЖДЕНИЯ ОТ УСТРОЙСТВ\n");  break;
        case 10:printf(" ОТПРАВЛЯЮ 03 ЕЩЕ РАЗ \n");                     break;
        case 11:printf(" ГОТОВ \n");                                    break;
    }
}
void PrintAddress(unsigned long address)
{
    printf(" %02lx %02lx %02lx %02lx ",(address>>24),(address>>16) & 255,(address>>8) & 255,(address) & 255);
}
void ShowRAMTable(WorkTable *ram)
{

    printf("\n\n|------------------------------ПАМЯТЬ УСТРОЙСТВА 0x%lx--------------------------------\n",ram->MAC);
    printf("|  Список потенциальных роутеров\n");
    int i = 0;
    while(ram->pRouterlist[i].address!=0){
        printf("|  %d\t|%d\t| ",ram->pRouterlist[i].device_counter, ram->pRouterlist[i].rssi); PrintAddress(ram->pRouterlist[i].address);printf("\n");
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
    printf("Основной и резервный роутер и их квитирование\n");
    printf("\nОсновной:   %d |    ",ram->my_routers[0].accept); PrintAddress(ram->my_routers[0].address);
    printf("\nРезервный:  %d |    ",ram->my_routers[1].accept); PrintAddress(ram->my_routers[1].address);
    printf("\n|------------------------------------------------------------------------------------------\n");
    printf("Кто выбрал меня роутером\n");
    i = 0;
    while(ram->i_main_router_for[i] != 0){
        PrintAddress(ram->i_main_router_for[i]); printf("\n");
        i++;
    }
    printf("|------------------------------------------------------------------------------------------\n");
    printf("Кто выбрал меня резервным роутером\n");
    i = 0;
    while(ram->i_reserve_router_for[i] != 0){
        PrintAddress(ram->i_reserve_router_for[i]); printf("\n");
        i++;
    }
    printf("|------------------------------------------------------------------------------------------\n");
}
void PrintPacket(Packet exmpl)
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n|-----------------------------------Входящий пакет-----------------------------------------\n");
    printf("|  Стартовый байт| %x \t|	Адрес узла отправителя	| ",exmpl._startpacket);		PrintAddress(exmpl._sourceaddres);        printf("\n");
    printf("|  Тип пакета    | %x \t|	Адрес узла получателя	| ",exmpl._typepacket);			PrintAddress(exmpl._destinationaddres);   printf("\n");
    printf("|----------------|-------------------------------------------------------------------------\n");
    printf("|  Уровень       | %x \t|	Адрес следующего узла	| ",exmpl._level);				PrintAddress(exmpl._nextaddres);          printf("\n");
    printf("|  Сессия        | %x \t|	Адрес предыдущего узла	| ",exmpl._session);			PrintAddress(exmpl._prevaddres);          printf("\n");
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
int PrintPacketLine(char *s,Packet exmpl)
{
    if(exmpl._startpacket != '$')
        return 0;
    printf("%s ПАКЕТ | ",s);
    printf("%c | %x | ",exmpl._startpacket ,exmpl._typepacket);
    PrintAddress(exmpl._sourceaddres);          printf("|");
    PrintAddress(exmpl._destinationaddres);     printf("|");
    printf("%x | %x | %x | %x | %x | %x | %x |",exmpl._synctime,exmpl._level,exmpl._session,exmpl._seance,exmpl._nodestate,exmpl._ordernumder,exmpl._ttl);
    PrintAddress(exmpl._nextaddres);            printf("|");
    PrintAddress(exmpl._prevaddres);            printf("|");
    printf(" %x |",exmpl._reserve);
    int i = 0;
    while(exmpl._payload[i]!='#') {
        printf("%02x", exmpl._payload[i]);
        printf(" ");
        i++;
    }
    printf("\n");
}

