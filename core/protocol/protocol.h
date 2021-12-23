#ifndef     RADIOROUTING_PROTOCOL_H
#define     RADIOROUTING_PROTOCOL_H

#define 	MAX_POTENTIAL_ROUTER         10                                                                                         //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО РОУТЕРЕРОВ СПОСОБНЫХ ХРАНИТСЯ В ПАМЯТИ ДЛЯ ПОДСЧЕТА
#define 	MAX_SUB_ROUTERS              10                                                                                              //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ РОУТЕРОВ
#define 	MAX_END_DEVICES              10                                                                                              //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ КОНЕЧНЫХ УСТРОЙСТВ
#define 	HEADER_LEN                   28                                                                                                   //ДЛИННА ЗАГОЛОВКА ПАКЕТА
#define 	LEN_PAYLOAD                  100                                                                                            //МАКСИМАЛЬНАЯ ДЛИННА ПОЛЕЗНОЙ НАГРУЗКИ
#define     MAIN_ROUTER                  0
#define     ALTERNATE_ROUTER             1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define     DELAY_OF_SLEEP                                                  15
#define     DELAY_OF_START_DEFINING_ROUTERS                                 15
#define     DELAY_OF_ROUTER_IS_DEFINED                                      10
#define     DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER                          10
#define     DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS                   10
#define     DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES             10
#define     DELAY_OF_ANNOUNCEMENT_ROUTER_STATUS                             10
#define     DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES  10

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------ПЕРЕЧИСЛЕНИЯ------------------------
typedef enum {
    //////////////////ПЕРВЫЙ ЭТАП
    SLEEP,                                                                                                                  //СОСТОЯНИЕ ОЖИДАНИЯ КОМАНД НА ПОСТРОЕНИЕ СЕТИ (ОЖИДАНИЕ ПАКЕТА 00)
    SEND_01,
    START_DEFINING_ROUTERS,                                                                                                 //ОЖИДАЕТ ПАКЕТЫ ОТ СОСЕДЕЙ, ФОРМИРУЕТ ТАБЛИЦУ И ВЫБИРАЕТ СВОИ РОДИТЕЛЬСКИЕ РОУТЕРЫ
    SEND_02,
    ROUTER_IS_DEFINED,                                                                                                      //СОХРАНЯЕТ АДРЕС УСТРОЙСТВА В СПРИСОК УСТРОЙСТВ ОТ КОТОРЫХ ОЖИДАЕТСЯ ПОДТВЕРЖДЕНИЕ
    CONFIRM_FROM_POTENTIAL_ROUTER,                                                                                          //ОЖИДАНИЕ ПОЛУЧЕНИЯ ПОДТВЕРЖДЕНИЯ
    ////////////////// ВТОРОЙ ЭТАП
    ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS,                                                                                   //ОТПРАВКА ПАКЕТА 00
    WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                                                                             //ОЖИДАНИЕ ПАКЕТА 02
    ANNOUNCEMENT_ROUTER_STATUS,                                                                                             //ОТПРАВКА ПАКЕТА 03
    ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                                                                  //ПОВТОРНОЕ ОЖИДАНИЕ ПАКЕТОВ 02
    ////////////////// РАБОЧЕЕ СОСТОЯНИЕ
    READY,                                                                                                                  //ГОТОВ К ОПРОСУ СЕТИ
    RETRANSLATE,
    MY_UNO_IS_PACKED


    } STATE;                                                                                                   //СОСТОЯНИЕ УЗЛА, НА ОСНОВАНИИ КОТОРОГО ОТБРАСЫВАЮТСЯ ПАКЕТЫ
typedef enum {
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;                                                                                                    //РОЛЬ УЗЛА - ШЛЮЗ, РОУТЕР, ИЛИ КОНЕЧНЫЙ УЗЕЛ
//----------------------СТРУКТУРЫ--------------------------
typedef struct  Packet {                                                                                                    //РАЗБИТЫЙ ЗАГОЛОВОК ПАКЕТА, ЕГО НАГРУЗКА И НЕКОТОРАЯ СЛУЖЕБНАЯ ИНФОРМАЦИЯ

    unsigned char 	_startpacket; 				                                                                            // [1 byte] symbol of start "$"
    unsigned char	_typepacket;				                                                                            // [1 byte] code defining the type of package

    unsigned long	_sourceaddres;				                                                                            // [4 byte] address of source node
    unsigned long	_destinationaddres;			                                                                            // [4 byte] address of destination node

    unsigned short	_synctime;					                                                                            // [2 byte] sync time
    unsigned char	_session;					                                                                            // [1 byte] code session
    unsigned char	_level;						                                                                            // [1 byte] node level
    unsigned char	_seance;					                                                                            // [1 byte] statistics field
    unsigned char	_nodestate;					                                                                            // [1 byte] Router/Alternative Router/End Node
    unsigned char	_ordernumder;				                                                                            // [1 byte] number of packet in order
    unsigned char	_ttl;						                                                                            // [1 byte] No comments

    unsigned long 	_nextaddres;				                                                                            // [4 byte] address of next node
    unsigned long 	_prevaddres;				                                                                            // [4 byte] address of previous node

    unsigned short 	_reserve;					                                                                            // [2 byte] RESERVE
    unsigned char	_payload[LEN_PAYLOAD];				                                                                            // [100 byte] Payload. End of payload must be sym. #
    unsigned int    _plen;                                                                                                  // [4 byte] len


}    Packet;                                                                                     //СТРУКТУРА ЗАГОЛОВОК ПАКЕТА, ЕГО НАГРУЗКИ И НЕКОТОРОЙ СЛУЖЕБНОЙ ИНФОРМАЦИИ
typedef struct  RouteUnit {

    unsigned long 	            address;
    unsigned int				device_counter;
    unsigned int                rssi;

} RouteUnit;                                                                                  //СТРУКТУРА ИМИТИРУЮЩАЯ АССОЦИАТИВНЫЙ СПИСОК ХРАНЯЩИЙ АДРЕС УСТРОЙСТВ И КОЛИЧЕСТВО ЕГО ПОВТОРЕНИЙ
typedef struct  WorkTable {

    /////////////   ДАННЫЕ С ДАТЧИКОВ (ПРИЛОЖЕНИЯ)
    unsigned char   many_payload[LEN_PAYLOAD];
    unsigned int    len_of_list;
    /////////////   ТАБЛИЦА ПОСТРОЕНИЯ СЕТИ
    RouteUnit	 	pRouterlist[MAX_POTENTIAL_ROUTER];
    /////////////   НАСТРОЙКИ УСТРОЙСТВА
    ROLE            Device;
    STATE           Status;
    unsigned long   MAC;
    /////////////   ДАННЫЕ В РАМКАХ СЕТИ
    unsigned char   my_role;
    unsigned char	my_session;
    unsigned char	my_level;
    unsigned short	my_time;
    unsigned char	my_seance;
    unsigned char   packet_order;
    /////////////   РАБОЧИЕ ТАБЛИЦЫ
    unsigned long   gateway;
    unsigned long   temporary_prev_address;
    unsigned long	my_subrouters[MAX_SUB_ROUTERS];
    unsigned long	my_routers[2];
    unsigned long	i_reserve_router_from[MAX_SUB_ROUTERS];
    unsigned long	my_end_devices[MAX_END_DEVICES];
    Packet          output_packet;
    char            output_payload[LEN_PAYLOAD];
    /////////////   ВРЕМЯ
    unsigned int    delta_time;
    unsigned int    start_status_time;
    //ВРЕМЯ НАХОЖДЕНИЯ УСТРОЙСТВА В АКТУАЛЬНОМ СТАТУСЕ
} WorkTable;                                                                                  //СТРУКТУРА ПАМЯТИ ЛЮБОГО УСТРОЙСТВА
//---------------------ИНСТРУМЕНТЫ-------------------------
unsigned long   GetAddress(const unsigned char *stream, int startbyte);                                                     //ВЫТЯГИВАЕТ ИЗ ПОТОКА CHAR* АДРЕСА В ФОРМАТЕ ULONG
void            GetAddressChar(char * buff, unsigned long stream);
//------------------------МЕТОДЫ---------------------------
int             SetDefault();
Packet 			ParcerHeader(const unsigned char *stream);						                                            //ПАРСЕР ЗАГОЛОВКА
void			PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *stream);			            //ОСНОВНОЙ МЕНЕДЖЕР
void            ServiceFieldAdding(WorkTable *ram,Packet pack);                                                             //РАБОТА С ДОУГИМИ СЕРВИСНЫМИ ПОЛЯМИ ЗАГОЛОВКА
unsigned char   VALIDATOR(WorkTable * ram, Packet pack);                                                                    //МЕТОД ОТБРАСЫВАЮЩИЙ ПАКЕТЫ КОТОРЫЕ НЕ НАЗНАЧАЛИСЬ УСТРОЙСТВУ
int             getCurrentState();                                                                                          //ВЫТЯГИВАЕМ СОСТОЯНИЕ ДЛЯ ФОРМИРОВАНИЯ НЕОБХОДИМОГО ПАКЕТА
void            packetConstructor(WorkTable *ram, unsigned char   _startpacket, unsigned char	_typepacket, unsigned long	_sourceaddres, unsigned long	_destinationaddres, unsigned short	_synctime, unsigned char	_session, unsigned char	_level, unsigned char	_seance, unsigned char	_nodestate, unsigned char	_ordernumder, unsigned char	_ttl, unsigned long 	_nextaddres, unsigned long 	_prevaddres, unsigned short 	_reserve, unsigned char	*_payload);
#endif //RADIOROUTING_PROTOCOL_H
