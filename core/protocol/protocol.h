/**
 *  Данный заголовочный файл содержит объявление основных полей и функций, обеспечивающих работу _Протокола_.
 *  Програмный модуль, реализующий все необходимые для работы _Протокола_ механизмы, (далее "Протокол") состоит
 *  из следующих составляющих:
 *  1) Рабочая таблица (WorkTable) - содержит всю служебную информацию, необходимую для функционирования _Протокола_,
 *                                  в частности: адреса основного и резервного роутеров, список адресов подчинённых
 *                                  роутеров, очередь исходящих пакетов, данные о положении устройства в рамках сети,
 *                                  настройки устройства и данные для передачи по сети;
 *
 *  2) Менеджер пакетов  - основная функция, вокруг которой строится вся логика работы "Протокола".
 *                         Менеджер пакетов устанавливает порядок и условия запуска вспомогательных модулей,
 *                         обеспечивающих взаимодействия данного устройства с сетью, работающей на базе _Протокола_;
 *
 *  3) Парсер заголовков входящих пакетов (ParseHeader) - вспомогательный модуль, который представляет собой функцию,
 *                                                        извлекающей из байтового массива поля заголовка пакетов и
 *                                                        помещает их в специальную структуру типа Packet;
 *
 *  4) Валидатор входящих пакетов (Validator) - вспомогательный модуль, который является функцией, выполняющей
 *                                              проверку входящего пакета на целостность (наличие символа начала и конца
 *                                              пакета, размер пакета и т.д.), а также на соответсвие типа входящего
 *                                              пакета с разрешённым набором типов, установленных для текущего статуса,
 *                                              в котором пребывает устройство.
 *
 *
 *  5) Набор обработчиков входящих пакетов (packet_Handler_xx) - набор функций для обработки входящих пакетов,
 *                                                               обновления сведений о сети, обновления текущих настроек
 *                                                               на основе информации полученной из обработанного пакета.
 *
 *  6) Котроллер состояний узла сети/устройства (StatusController)
 *  7) Набор фабрик исходящих пакетов (packet_Factory_xx)
 *  8) Менеджер очереди исходящих пакетов (QueueManager)
 *
 *
 *
 *
 *
 *
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H



//////////////////////////////////////////////////////////////////////////////
#define     MAX_POTENTIAL_ROUTER                                            5                                               //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО РОУТЕРОВ СПОСОБНЫХ ХРАНИТСЯ В ПАМЯТИ ДЛЯ ПОДСЧЕТА
#define     MAX_RESERVE_SUBROUTERS                                          5                                               //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ РОУТЕРОВ
#define     MAX_MAIN_SUBROUTERS                                             5                                               //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ КОНЕЧНЫХ УСТРОЙСТВ
#define     HEADER_LEN                                                      28                                              //ДЛИННА ЗАГОЛОВКА ПАКЕТА
#define     LEN_PAYLOAD                                                     100                                             //МАКСИМАЛЬНАЯ ДЛИННА ПОЛЕЗНОЙ НАГРУЗКИ
#define     MAIN_ROUTER                                                     0                                               //ИТЕРАТОР ОСНОВОНОГО РОУТЕРА В ТАБЛИЦАХ ДЛЯ УДОБСТВА
#define     RESERVE_ROUTER                                                  1                                               //ИТЕРАТОР РЕЗЕРВНОГО РОУТЕРА В ТАБЛИЦАХ ДЛЯ УДОБСТВА
//////////////////////////////////////////////////////////////////////////////
#define     DELAY_OF_SLEEP                                                  5
#define     DELAY_OF_START_DEFINING_ROUTERS                                 5
#define     DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER                          5
#define     DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS                   5
#define     DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES             5
#define     DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES  5


////////////////////////////ПЕРЕЧИСЛЕНИЯ/////////////////////////
typedef enum
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
typedef enum
{
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;
////////////////////////////СТРУКТУРЫ////////////////////////////
typedef struct  AcceptedRouter
{
    unsigned int  accept;
    unsigned long address;

} AcceptedRouter;
typedef struct  Packet
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
typedef struct  RouteUnit
{
    unsigned long   address;                                                              //АДРЕС УСТРОЙСТВА
    unsigned int    device_counter;
    unsigned int    rssi;

} RouteUnit;
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
    unsigned long   start_status_time;
} WorkTable;
///////////////////////ОСНОВНОЙ МЕНЕДЖЕР/////////////////////////
void            PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *stream);
///////////////ОСНОВНЫЕ ПРОЦЕДУРЫ МЕНЕДЖЕРА/////////////////////
Packet          ParseHeader(const unsigned char *stream);
unsigned char   Validator(WorkTable * ram, Packet pack);
void            StatusController(WorkTable * ram);
void            QueueManager(WorkTable *ram);
/////////////////////////ИНСТРУМЕНТЫ/////////////////////////////
unsigned long   GetRandomAddress();
unsigned long   GetAddress(const unsigned char *stream, int startbyte);
void            GetAddressChar(char * buff, unsigned long stream);
void            packetConstructor(WorkTable *ram,unsigned char _startpacket,unsigned char _typepacket,unsigned long _sourceaddres,unsigned long _destinationaddres,unsigned short _synctime,unsigned char _level,unsigned char _session,unsigned char _seance,unsigned char _nodestate,unsigned char _ordernumder,unsigned char _ttl,unsigned long _nextaddres,unsigned long _prevaddres,unsigned short _reserve,const unsigned char *_payload);
///////////////////////РАБОЧИЕ АЛГОРИТМЫ/////////////////////////
void            DefiningRouters(WorkTable *ram);
int             isTimeout(WorkTable *ram, unsigned int delay);
void            SetDefault(WorkTable *ram);
void            Queue_up(WorkTable *ram, unsigned int repeat, unsigned int time_to_send, Packet exmpl);                     //ВСТАТЬ В ОЧЕРЕДЬ//СБРОС УСТРОЙСТВА
void            ServiceFieldAdding(WorkTable *ram,Packet pack);                                                             //РАБОТА С ДРУГИМИ СЕРВИСНЫМИ ПОЛЯМИ ЗАГОЛОВКА
/////////////////////////ОБРАБОТЧИКИ/////////////////////////////
void 			packet_Handler_00(WorkTable * ram, Packet pack, int RSSI);                                                      //ОБРАБОТЧИК ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void			packet_Handler_01(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "Я УЗЕЛ"
void 			packet_Handler_02(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void 			packet_Handler_03(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "Я РОУТЕР"
void 			packet_Handler_04(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "ОПРОС УСТРОЙСТВ"
void 			packet_Handler_05(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void 			packet_Handler_06(WorkTable * ram, Packet pack);                                                                //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"
/////////////////////////ФАБРИКИ////////////////////////////////
void            packet_Factory_00(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_01(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_02(WorkTable * ram);                                                                       	//ФАБРИКА ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void            packet_Factory_03(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я РОУТЕР"
void            packet_Factory_04(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОПРОС УСТРОЙСТВ"
void            packet_Factory_05(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void            packet_Factory_06(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"
////////////////////////////ПЕРЕМЕННЫЕ/////////////////////////
WorkTable       RAM;                                        //Память, выделяемая под логику протокола
int             RSSI;                                       //Значение урвня принятого сигнала
///////////////////////////////////////////////////////////////
#endif //RADIOROUTING_PROTOCOL_H
