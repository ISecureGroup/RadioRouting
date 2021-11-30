#ifndef     RADIOROUTING_PROTOCOL_H
#define     RADIOROUTING_PROTOCOL_H

#define 	MAX_POTENTIAL_ROUTER 10                                                                                         //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО РОУТЕРЕРОВ СПОСОБНЫХ ХРАНИТСЯ В ПАМЯТИ ДЛЯ ПОДСЧЕТА
#define 	MAX_SUB_ROUTERS 10                                                                                              //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ РОУТЕРОВ
#define 	MAX_END_DEVICES 10                                                                                              //МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ДОЧЕРНИХ КОНЕЧНЫХ УСТРОЙСТВ
#define 	HEADER_LEN 28                                                                                                   //ДЛИННА ЗАГОЛОВКА ПАКЕТА
#define 	LEN_PAYLOAD_MANY 100                                                                                            //МАКСИМАЛЬНАЯ ДЛИННА ПОЛЕЗНОЙ НАГРУЗКИ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------ПЕРЕЧИСЛЕНИЯ-----------------------
typedef enum {
    ////////////////// СОСТОЯНИЯ ПЕРВОГО ЭТАПА
    SLEEP,                                                                                                                  //СОСТОЯНИЕ ОЖИДАНИЯ КОМАНД НА ПОСТРОЕНИЕ СЕТИ
    AWAKING,                                                                                                                //УСТРОЙСТВО ПРОБУДИЛОСЬ
    LISTENING_MEDIUM,                                                                                                       //ПРОСЛУШИВАНИЕ ЭФИРА УСТРОЙСТВОМ ДЛЯ ОБЩЕНИЯ С ДРУГИМИ УСТРОЙСТВАМИ И ОПРЕДЕЛЕНИЯ СВОЕГО ОСНОВНОГО И РЕЗЕРВНОГО РОУТЕРА ДЛЯ ВСЕХ
    SELECT_ROUTERS,                                                                                                         //СОСТОЯНИЕ ВЫБОРА РОУТЕРА
    ROUTER_WAIT_ELECTION,                                                                                                   //РЕЖИМ ОЖИДАНИЯ РОУТЕРА, ДО ПОЛУЧЕНИЯ СВЕДЕНИЙ О ТОМ КТО ЕГО ВЫБРАЛ ОТ ВСЕХ УСТРОЙСТВ
    ////////////////// СОСТОЯНИЯ ВТОРОГО ЭТАПА
    READY,                                                                                                                  //СОСТОЯНИЕ ГОТОВНОСТИ К ПЕРЕДАЧЕ
    UNO_MANY,                                                                                                               //ДОЛЖЕН БУДЕТ БЫТЬ РЕЖИМ В КОТОРОМ УЖЕ ВОСПРИНИМАЮТСЯ ПАКЕТЫ УНО ИЛИ МЕНИ
    ////////////////// РАБОЧЕЕ СОСТОЯНИЕ
    RETRANSLATE,                                                                                                            //СТАТУС ПОКА ТЕСТИРУЕТСЯ
    MY_UNO_IS_PACKED                                                                                                        //СТАТУС ПОКА ТЕСТИРУЕТСЯ

    } STATE;                                                                                                   //СОСТОЯНИЕ УЗЛА, НА ОСНОВАНИИ КОТОРОГО ОТБРАСЫВАЮТСЯ ПАКЕТЫ
typedef enum {
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;                                                                                                    //РОЛЬ УЗЛА - ШЛЮЗ, РОУТЕР, ИЛИ КОНЕЧНЫЙ УЗЕЛ
//------------------------СТРУКТУРЫ------------------------
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
    unsigned char	_payload[100];				                                                                            // [100 byte] Payload. End of payload must be sym. #
    unsigned int    _plen;                                                                                                  // [4 byte] len


}    Packet;                                                                                     //СТРУКТУРА ЗАГОЛОВОК ПАКЕТА, ЕГО НАГРУЗКИ И НЕКОТОРОЙ СЛУЖЕБНОЙ ИНФОРМАЦИИ
typedef struct  RouteUnit {

    unsigned long 	            address;
    unsigned int				device_counter;

} RouteUnit;                                                                                  //СТРУКТУРА ИМИТИРУЮЩАЯ АССОЦИАТИВНЫЙ СПИСОК ХРАНЯЩИЙ АДРЕС УСТРОЙСТВ И КОЛИЧЕСТВО ЕГО ПОВТОРЕНИЙ
typedef struct  WorkTable {

    /////////////   ДАННЫЕ С ДАТЧИКОВ (ПРИЛОЖЕНИЯ)
    unsigned char   many_payload[LEN_PAYLOAD_MANY];
    unsigned int    len_of_list;
    /////////////   ТАБЛИЦА ПОСТРОЕНИЯ СЕТИ
    RouteUnit	 	pRouterlist[MAX_POTENTIAL_ROUTER];
    /////////////   НАСТРОЙКИ УСТРОЙСТВА
    ROLE            DEVICE;
    STATE           STATUS;
    unsigned long   MAC;
    /////////////   ДАННЫЕ В РАМКАХ СЕТИ
    unsigned char	my_session;
    unsigned char	my_level;
    unsigned short	my_time;
    unsigned char	my_seance;
    /////////////   РАБОЧИЕ ТАБЛИЦЫ
    unsigned long	my_subrouters[MAX_SUB_ROUTERS];
    unsigned long	my_routers[2];
    unsigned long	i_reserve_router_from[MAX_SUB_ROUTERS];
    unsigned long	my_end_devices[MAX_END_DEVICES];
    Packet          output_packet;
} WorkTable;                                                                                  //СТРУКТУРА ПАМЯТИ ЛЮБОГО УСТРОЙСТВА
//------------------------МЕТОДЫ---------------------------
Packet 			ParcerHeader(const unsigned char *stream);						                                            //ПАРСЕР ЗАГОЛОВКА
void 			PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *stream);			            //ОСНОВНОЙ МЕНЕДЖЕР
unsigned long   GetAddress(const unsigned char *stream, int startbyte);                                                     //ВЫТЯГИВАЕТ ИЗ ПОТОКА CHAR* АДРЕСА В ФОРМАТЕ ULONG
void            ServiceFieldAdding(WorkTable *ram,Packet pack);                                                             //РАБОТА С ДОУГИМИ СЕРВИСНЫМИ ПОЛЯМИ ЗАГОЛОВКА
unsigned char   PacketValidator(WorkTable * ram, Packet pack);
int             getCurrentState();
//МЕТОД ОТБРАСЫВАЮЩИЙ ПАКЕТЫ КОТОРЫЕ НЕ НАЗНАЧАЛИСЬ УСТРОЙСТВУ
//------------------------FACTORY---------------------------
void            packet_Factory_00(WorkTable * ram);
void            packet_Factory_01(WorkTable * ram);
void            packet_Factory_02(WorkTable * ram);
void            packet_Factory_03(WorkTable * ram);
void            packet_Factory_04(WorkTable * ram);
void            packet_Factory_05(WorkTable * ram);
void            packet_Factory_06(WorkTable * ram);

//------------------------HANDLERS--------------------------
void 			pl_Handler_00(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void			pl_Handler_01(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я УЗЕЛ"
void 			pl_Handler_02(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void 			pl_Handler_03(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "Я РОУТЕР"
void 			pl_Handler_04(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОПРОС УСТРОЙСТВ"
void 			pl_Handler_05(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void 			pl_Handler_06(WorkTable * ram,Packet pack);                                                                 //ОБРАБОТЧИК ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"
//---------------------------------------------------------
#endif //RADIOROUTING_PROTOCOL_H
