/**
 *  Данный заголовочный файл содержит объявление основных полей и функций, обеспечивающих работу _Протокола_.
 *  Програмный модуль, реализующий все необходимые для работы _Протокола_ механизмы, (далее "Протокол") состоит
 *  из следующих составляющих:
 *
 *  1) Рабочая таблица (WorkTable) - содержит всю служебную информацию, необходимую для функционирования _Протокола_,
 *                                   в частности: адреса основного и резервного роутеров, список адресов подчинённых
 *                                   роутеров, очередь исходящих пакетов, данные о положении устройства в рамках сети,
 *                                   настройки устройства и данные для передачи по сети;
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
 *                                              в котором пребывает устройство;
 *
 *
 *  5) Набор обработчиков входящих пакетов (packet_Handler_xx) - набор функций для обработки входящих пакетов,
 *                                                               обновления сведений о сети, обновления текущих настроек
 *                                                               на основе информации полученной из обработанного пакета;
 *
 *  6) Контроллер состояний узла сети/устройства (StatusController) - вспомогательный модуль, являющейся функцией,
 *                                                                    определяющей условия и порядок перехода устройства
 *                                                                    в то или иное состояние;
 *
 *  7) Набор фабрик исходящих пакетов (packet_Factory_xx) - набор функций, отвечающих за генерацию исходящих (ответных)
 *                                                          пакетов, посредством которых устройство взаимодействует с
 *                                                          сетью, работающей на базе _Протокола_. Каждая функция в
 *                                                          результате своей работы возвращает пакет в виде байтового
 *                                                          массива.
 *
 *
 *  8) Менеджер очереди исходящих пакетов (QueueManager) - вспомогательный модуль, представляющий собой функцию, которая
 *                                                         определяет время отправки исходящих пакетов. Время отправки
 *                                                         определяется задержкой, которая генерируется по правилам,
 *                                                         установленных в _Протоколе_.
 *
 *
 * @authors Kutuzov V.I (kutuzovvladislav@mail.ru), Azizov R.R (leofoxi73@gmail.com), Elyagin S.V. (esv@ulstu.ru)
 * @version 1.0
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

//-----------------------------------------------НАСТРОЙКИ--------------------------------------------------------------
/**
 * Максимальное количество устройств сети, регистрируемых данным устройством
 */
#define     MAX_POTENTIAL_ROUTER                                            5
#define     MAX_RESERVE_SUBROUTERS                                          5
#define     MAX_LEN_OF_ROUTER_LIST                                          5
#define     MAX_DEVICES_FOR_WHICH_IM_RESERVE_ROUTER                         5
#define     MAX_DEVICES_FOR_WHICH_IM_MAIN_ROUTER                            5

/**
 * Размеры заголовка и полезной нагрузки(содержимого) пакета
 */
#define     HEADER_LEN                                                      28
#define     MAX_LEN_PAYLOAD                                                 100

/**
 * Размеры заголовка и полезной нагрузки(содержимого) пакета
 */
#define     MAIN_ROUTER                                                     0
#define     RESERVE_ROUTER                                                  1

/**
 * Задержки состояний (сколько времени устройство должно находится в том или ином состоянии)
 */
#define     DELAY_OF_SLEEP                                                  5
#define     DELAY_OF_START_DEFINING_ROUTERS                                 5
#define     DELAY_OF_CONFIRM_FROM_POTENTIAL_ROUTER                          5
#define     DELAY_OF_ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS                   5
#define     DELAY_OF_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES             5
#define     DELAY_OF_ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES  5


//---------------------------------------------РАБОЧИЕ ПОЛЯ-------------------------------------------------------------

/**
 * Перечисление, которое хранит все стадии(состояния) жизненного цикла _Протокола_
 */
typedef enum
{
    //ПЕРВАЯ ФАЗА ЭТАПА ПОСТРОЕНИЯ СЕТИ
    SEND_00,                                                                  //отправка пакета 00
    SLEEP,                                                                    //ожидание команды на построение сети (ожидание пакета 00)
    SEND_01,                                                                  //отправка пакета 01
    START_DEFINING_ROUTERS,                                                   //ожидание пакета 01 от соседей, определение основного и резервного роутеров
    SEND_02,                                                                  //отправка пакета 02
    WAIT_CONFIRM_FROM_POTENTIAL_ROUTER,                                       //ожидание пакета 03 от потенциального роутера для подтверждения доставки пакета 02

    //ВТОРАЯ ФАЗА ЭТАПА ПОСТРОЕНИЯ СЕТИ
    ANNOUNCEMENT_POTENTIAL_ROUTER_STATUS,                                     //объявление себя потенциальным роутером (отправка пакета 00)
    WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                               //ожидание пакетов 02
    SEND_03,                                                                  //отправка пакета 03
    ADDITIONAL_WAITING_CONFIRM_ROUTER_STATUS_FROM_DEVICES,                    //повторное ожидание пакетов 02
    SEND_03A,

    //ЭТАП ПЕРЕДАЧИ ДАННЫХ ПО СЕТИ
    READY                                                                     //отправка пакета 03 в дополнительное время
} STATE;

/**
 * Перечисление, которое хранит список ролей устройства
 */
typedef enum
{
    UNDEFINED,
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;

/**
 * Структура описывающая основные свойства вышестоящего(родительского) роутера
 */
typedef struct  UpRouter
{
    unsigned int  accept;
    unsigned long address;

} UpRouter;

/**
 * Структура пакетов, определяемая _Протоколом_
 */
typedef struct  Packet
{
    unsigned char   _startpacket; 				                             // [1 byte] символ начала пакета "$"
    unsigned char   _typepacket;				                             // [1 byte] тип пакета

    unsigned long   _sourceaddres;				                             // [4 byte] адрес отправителя пакета
    unsigned long   _destinationaddres;			                             // [4 byte] адрес назначения пакета

    unsigned short  _synctime;					                             // [2 byte] время синхронизации
    unsigned char   _session;					                             // [1 byte] код сессии (сессия - период работы сети в рамках одной топологии до перестроения)
    unsigned char   _level;						                             // [1 byte] уровень, на котором находится узел (число хопов до шлюза)
    unsigned char   _seance;					                             // [1 byte] код сеанса (сеанс - период работы сети, когда происходит опрос устройств. При сбросе сессии сеанс тоже сбрасывается)
    unsigned char   _nodestate;					                             // [1 byte] состояние узла (роутер или оконечное устройство)
    unsigned char   _ordernumder;				                             // [1 byte] порядковый номер пакета, отправленного устройства
    unsigned char   _ttl;						                             // [1 byte] оставшееся число ретрансляций пакета

    unsigned long   _nextaddres;				                             // [4 byte] адрес следующего промежуточного узла
    unsigned long   _prevaddres;				                             // [4 byte] адрес предыдущего узла, отправившего пакет

    unsigned short  _reserve;					                             // [2 byte] резервное поле, для доп. функций на будущее
    unsigned char   _payload[MAX_LEN_PAYLOAD];				                 // [100 byte] Передаваемые данные
    unsigned int    payload_len;                                             // [4 byte] длинна поля данных пакета (передаваемых данных)
} Packet;

/**
 * Структура записи таблицы очередей
 */
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
    unsigned char   many_payload[MAX_LEN_PAYLOAD];
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
    UpRouter  my_routers[2];
    unsigned long   i_reserve_router_for[MAX_DEVICES_FOR_WHICH_IM_RESERVE_ROUTER];
    unsigned long   i_main_router_for[MAX_DEVICES_FOR_WHICH_IM_MAIN_ROUTER];
    Packet          output_packet;
    unsigned char   output_payload[MAX_LEN_PAYLOAD];

    /////////////   ВРЕМЯ И ОЧЕРЕДЬ
    struct qUnit    QUEUE[5];
    unsigned long   current_time;
    unsigned long   delta_time;
    unsigned long   start_status_time;
} WorkTable;
///////////////////////ОСНОВНОЙ МЕНЕДЖЕР/////////////////////////
int            PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *instream, unsigned char *outstream);
///////////////ОСНОВНЫЕ ПРОЦЕДУРЫ МЕНЕДЖЕРА/////////////////////
Packet          ParseHeader(const unsigned char *stream);
unsigned char   Validator(WorkTable * ram, Packet pack);
void            StatusController(WorkTable * ram);
void            QueueManager(WorkTable *ram);
/////////////////////////ИНСТРУМЕНТЫ/////////////////////////////
unsigned long   GetRandomAddress();
unsigned long   GetAddress(const unsigned char *stream, int startbyte);
void            GetAddressChar(char * buff, unsigned long stream);
void            packetConstructor(WorkTable *ram,
                                  unsigned char _startpacket,
                                  unsigned char _typepacket,
                                  unsigned long _sourceaddres,
                                  unsigned long _destinationaddres,
                                  unsigned short _synctime,
                                  unsigned char _level,
                                  unsigned char _session,
                                  unsigned char _seance,
                                  unsigned char _nodestate,
                                  unsigned char _ordernumder,
                                  unsigned char _ttl,
                                  unsigned long _nextaddres,
                                  unsigned long _prevaddres,
                                  unsigned short _reserve,
                                  const unsigned char *_payload);
///////////////////////РАБОЧИЕ АЛГОРИТМЫ/////////////////////////
void            DefiningRouters(WorkTable *ram);
int             isTimeout(WorkTable *ram, unsigned int delay);
void            SetDefault(WorkTable *ram);
void            Queue_up(WorkTable *ram, unsigned int repeat, unsigned int time_to_send, Packet exmpl);                     //ВСТАТЬ В ОЧЕРЕДЬ//СБРОС УСТРОЙСТВА
void            ServiceFieldAdding(WorkTable *ram,Packet pack);                                                             //РАБОТА С ДРУГИМИ СЕРВИСНЫМИ ПОЛЯМИ ЗАГОЛОВКА
void            StartInitProtocol(WorkTable *ram);
/////////////////////////ОБРАБОТЧИКИ/////////////////////////////
void 			packet_Handler_00(WorkTable * ram, Packet pack, int RSSI);                                                  //ОБРАБОТЧИК ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void			packet_Handler_01(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "Я УЗЕЛ"
void 			packet_Handler_02(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void 			packet_Handler_03(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "Я РОУТЕР"
void 			packet_Handler_04(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "ОПРОС УСТРОЙСТВ"
void 			packet_Handler_05(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void 			packet_Handler_06(WorkTable * ram, Packet pack);                                                            //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"
/////////////////////////ФАБРИКИ////////////////////////////////
void            packet_Factory_00(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_01(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_02(WorkTable * ram);                                                                       	//ФАБРИКА ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void            packet_Factory_03(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я РОУТЕР"
void            packet_Factory_04(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОПРОС УСТРОЙСТВ"
void            packet_Factory_05(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void            packet_Factory_06(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"

#endif //RADIOROUTING_PROTOCOL_H
