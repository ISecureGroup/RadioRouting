#ifndef PROTOCOL_H
#define PROTOCOL_H
//////////////////////////////////////////////////////////////////////////////
#define     MAX_POTENTIAL_ROUTER                                            5            //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО РОУТЕРЕРОВ СПОСОБНЫХ ХРАНИТСЯ В ПАМЯТИ ДЛЯ ПОДСЧЕТА
#define     MAX_RESERVE_SUBROUTERS                                          5            //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ РОУТЕРОВ
#define     MAX_MAIN_SUBROUTERS                                             5            //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ КОНЕЧНЫХ УСТРОЙСТВ
#define     HEADER_LEN                                                      28           //ДЛИННА ЗАГОЛОВКА ПАКЕТА
#define     LEN_PAYLOAD                                                     100          //МАКСИМАЛЬНАЯ ДЛИННА ПОЛЕЗНОЙ НАГРУЗКИ
#define     MAIN_ROUTER                                                     0            //ИТЕРАТОР ОСНОВОНОГО РОУТЕРА В ТАБЛИЦАХ ДЛЯ УДОБСТВА
#define     RESERVE_ROUTER                                                  1            //ИТЕРАТОР РЕЗЕРВНОГО РОУТЕРА В ТАБЛИЦАХ ДЛЯ УДОБСТВА
//////////////////////////////////////////////////////////////////////////////
#define     DELAY_OF_SLEEP                                                  10
#define     DELAY_OF_START_DEFINING_ROUTERS                                 10
#define     DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER                          10
#define     DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS                   10
#define     DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES             10
#define     DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES  10
//////////////////////////////////////////////////////////////////////////////
typedef enum                                                                             //СОСТОЯНИЕ УЗЛА, НА ОСНОВАНИИ КОТОРОГО ОТБРАСЫВАЮТСЯ ПАКЕТЫ
{
    //////////////////ПЕРВЫЙ ЭТАП//////////////////////////////
    SEND_00,                                                                             //ОТПРАВКА ПАКЕТА 00
    SLEEP,                                                                               //СОСТОЯНИЕ ОЖИДАНИЯ КОМАНД НА ПОСТРОЕНИЕ СЕТИ (ОЖИДАНИЕ ПАКЕТА 00)
    SEND_01,                                                                             //ОТПРАВКА ПАКЕТА 01
    START_DEFINING_ROUTERS,                                                              //ОЖИДАЕТ ПАКЕТЫ ОТ СОСЕДЕЙ, ФОРМИРУЕТ ТАБЛИЦУ И ВЫБИРАЕТ СВОИ РОДИТЕЛЬСКИЕ РОУТЕРЫ
    SEND_02,                                                                             //СОХРАНЯЕТ АДРЕС УСТРОЙСТВА В СПРИСОК УСТРОЙСТВ ОТ КОТОРЫХ ОЖИДАЕТСЯ ПОДТВЕРЖДЕНИЕ
    WAIT_CONFIRM_FROM_POTENTIAL_ROUTER,                                                  //ОЖИДАНИЕ ПОЛУЧЕНИЯ ПОДТВЕРЖДЕНИЯ
    ////////////////// ВТОРОЙ ЭТАП/////////////////////////////
    ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS,                                                //ОБЪЯВЛЕНИЕ СЕБЯ РОУТЕРОМ
    WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                                          //ОЖИДАНИЕ ПАКЕТА 02
    SEND_03,                                                                             //ОТПРАВКА ПАКЕТА 03
    ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                               //ПОВТОРНОЕ ОЖИДАНИЕ ПАКЕТОВ 02
    SEND_03A,
    ////////////////// РАБОЧЕЕ СОСТОЯНИЕ///////////////////////
    READY                                                                                //ГОТОВ К ОПРОСУ СЕТИ
} STATE;
typedef enum                                                                             //РОЛЬ УЗЛА - ШЛЮЗ, РОУТЕР, ИЛИ КОНЕЧНЫЙ УЗЕЛ
{
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;
//----------------------СТРУКТУРЫ--------------------------
typedef struct  AcceptedRouter
{
    unsigned int  accept;
    unsigned long address;

} AcceptedRouter;

typedef struct  Packet                                                                   //РАЗБИТЫЙ ЗАГОЛОВОК ПАКЕТА, ЕГО НАГРУЗКА И НЕКОТОРАЯ СЛУЖЕБНАЯ ИНФОРМАЦИЯ
{
    unsigned char   _startpacket; 				                                         // [1 byte] symbol of start "$"
    unsigned char   _typepacket;				                                         // [1 byte] code defining the type of package

    unsigned long   _sourceaddres;				                                         // [4 byte] address of source node
    unsigned long   _destinationaddres;			                                         // [4 byte] address of destination node

    unsigned short  _synctime;					                                         // [2 byte] sync time
    unsigned char   _session;					                                         // [1 byte] code session
    unsigned char   _level;						                                         // [1 byte] node level
    unsigned char   _seance;					                                         // [1 byte] statistics field
    unsigned char   _nodestate;					                                         // [1 byte] Router/Alternative Router/End Node
    unsigned char   _ordernumder;				                                         // [1 byte] number of packet in order
    unsigned char   _ttl;						                                         // [1 byte] No comments

    unsigned long   _nextaddres;				                                         // [4 byte] address of next node
    unsigned long   _prevaddres;				                                         // [4 byte] address of previous node

    unsigned short  _reserve;					                                         // [2 byte] RESERVE
    unsigned char   _payload[LEN_PAYLOAD];				                                 // [100 byte] Payload. End of payload must be sym. #
    unsigned int    _plen;                                                               // [4 byte] len
} Packet;

typedef struct  qUnit
{
    unsigned int    isDelivered;                                                          //ФЛАГ ДОСТАВКИ ПАКЕТА
    unsigned short  time_to_send;                                                         //ВРЕМЯ ЖИЗНИ ПАКЕТА В ОЧЕРЕДИ
    unsigned int    repeat;                                                               //КОЛИЧЕСТВО ПОВТОРЕНИЙ
    Packet          q_packet;                                                             //САМ ПАКЕТ В ОЧЕРЕДИ
} qUnit;
typedef struct  RouteUnit                                                                 //ЮНИТ
{
    unsigned long   address;                                                              //АДРЕС УСТРОЙСТВА
    unsigned int    device_counter;
    unsigned int    rssi;

} RouteUnit;                                                                             //СТРУКТУРА ИМИТИРУЮЩАЯ АССОЦИАТИВНЫЙ СПИСОК ХРАНЯЩИЙ АДРЕС УСТРОЙСТВ И КОЛИЧЕСТВО ЕГО ПОВТОРЕНИЙ
typedef struct  WorkTable
{
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
    unsigned char   my_session;
    unsigned char   my_level;
    unsigned short  my_time;
    unsigned char   my_seance;
    unsigned char   packet_order;

    /////////////   РАБОЧИЕ ТАБЛИЦЫ
    unsigned long   gateway;
    unsigned long   temporary_prev_address;
    unsigned long   my_subrouters[MAX_MAIN_SUBROUTERS];
    AcceptedRouter  my_routers[2];
    unsigned long   i_reserve_router_from[MAX_RESERVE_SUBROUTERS];
    unsigned long   i_main_router_from[MAX_MAIN_SUBROUTERS];
    Packet          output_packet;
    unsigned char   output_payload[LEN_PAYLOAD];

    /////////////   ВРЕМЯ И ОЧЕРЕДЬ
    struct qUnit    QUEUE[5];
    unsigned long   current_time;
    unsigned long   delta_time;
    unsigned long   start_status_time;                                                                  //ВРЕМЯ НАХОЖДЕНИЯ УСТРОЙСТВА В АКТУАЛЬНОМ СТАТУСЕ
} WorkTable;
//---------------------ИНСТРУМЕНТЫ-------------------------
unsigned long   GetRandomAddress();
unsigned long   GetAddress(const unsigned char *stream, int startbyte);                                 //ВЫТЯГИВАЕТ ИЗ ПОТОКА CHAR* АДРЕСА В ФОРМАТЕ ULONG
void            GetAddressChar(char * buff, unsigned long stream);                                      //КОНВЕРТИРУЕТ ULONG АДРЕСС В МАССИВ ЧАРОВ
//------------------------МЕТОДЫ---------------------------
void            SetDefault(WorkTable *ram);                                                             //СБРОС УСТРОЙСТВА
Packet          ParcerHeader(const unsigned char *stream);                                              //ПАРСЕР ЗАГОЛОВКА
void            PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *stream);   //ОСНОВНОЙ МЕНЕДЖЕР
void            ServiceFieldAdding(WorkTable *ram,Packet pack);                                         //РАБОТА С ДОУГИМИ СЕРВИСНЫМИ ПОЛЯМИ ЗАГОЛОВКА
unsigned char   VALIDATOR(WorkTable * ram, Packet pack);                                                //МЕТОД ОТБРАСЫВАЮЩИЙ ПАКЕТЫ КОТОРЫЕ НЕ НАЗНАЧАЛИСЬ УСТРОЙСТВУ
void            QUEUE_MANAGER(WorkTable *ram);                                                          //МЕНЕДЖЕР ОЧЕРЕДЕЙ
void            Queue_up(WorkTable *ram, unsigned int repeat, unsigned int time_to_send, Packet exmpl); //ВСТАТЬ В ОЧЕРЕДЬ

#endif //RADIOROUTING_PROTOCOL_H
