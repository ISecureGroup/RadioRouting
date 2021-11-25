#ifndef RADIOROUTING_PROTOCOL_H
#define RADIOROUTING_PROTOCOL_H


#define 	MAX_POTENTIAL_ROUTER 10
#define 	MAX_SUB_ROUTERS 10
#define 	MAX_END_DEVICES 10
#define 	HEADER_LEN 28
#define 	LEN_PAYLOAD_MANY 100
#define 	CONST_LEN_SENSOR_DATA 20



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------ПЕРЕЧИСЛЕНИЯ----------------------------
typedef enum {
    SLEEP,
    DECLARING_MY_POT_ROUTERS,
    ROUTER_WAIT_ASK,
    SELECT_ROUTERS,
    DECLARING_RESULT_OF_SELECT,
    BIND_ACCEPTED,
    BIND_NOT_CONFIRMED,
    I_FIND_MY_ADDRESS,
    MY_UNO_IS_PACKED,
    RETRANSLATE
    } STATE;
typedef enum {
    END_NODE,
    ROUTER,
    GATEWAY
} ROLE;
//------------------------СТРУКТУРЫ-----------------------------
typedef struct  Packet {

    unsigned char 	_startpacket; 				// [1 byte] symbol of start "$"
    unsigned char	_typepacket;				// [1 byte] code defining the type of package

    unsigned long	_sourceaddres;				// [4 byte] address of source node
    unsigned long	_destinationaddres;			// [4 byte] address of destination node

    unsigned short	_synctime;					// [2 byte] sync time
    unsigned char	_level;						// [1 byte] node level
    unsigned char	_session;					// [1 byte] code session
    unsigned char	_seance;					// [1 byte] statistics field
    unsigned char	_nodestate;					// [1 byte] Router/Alternative Router/End Node
    unsigned char	_ordernumder;				// [1 byte] number of packet in order
    unsigned char	_ttl;						// [1 byte] No comments

    unsigned long 	_nextaddres;				// [4 byte] address of next node
    unsigned long 	_prevaddres;				// [4 byte] address of previous node

    unsigned short 	_reserve;					// [2 byte] RESERVE
    unsigned char	_payload[100];				// [100 byte] Payload. End of payload must be sym. #
    unsigned int    _plen;                      // [4 byte] len


}    Packet;
typedef struct  RouteUnit {

    unsigned long 	            address;
    unsigned int				device_counter;

} RouteUnit;
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
} WorkTable;
//----------------------МЕТОДЫ----------------------------------
Packet 			ParcerHeader(const unsigned char *stream);						// Parcer of input stream to struct
void 			PacketManager(unsigned char *sens, int RSSI, WorkTable * ram, unsigned char *stream);			// Analyzing type of packet
unsigned long   GetAddress(const unsigned char *stream, int startbyte);
void            ServiceFieldAdding(WorkTable *ram,Packet pack);
unsigned char   PacketValidator(WorkTable * ram, Packet pack);
//--------------------------------------------------------------
void 			pl_Handler_00(WorkTable * ram,Packet pack);
void			pl_Handler_01(WorkTable * ram,Packet pack);
void 			pl_Handler_02(WorkTable * ram,Packet pack);
void 			pl_Handler_03(WorkTable * ram,Packet pack);
void 			pl_Handler_04(WorkTable * ram,Packet pack);
void 			pl_Handler_05(WorkTable * ram,Packet pack);
void 			pl_Handler_06(WorkTable * ram,Packet pack);
//--------------------------------------------------------------

#endif //RADIOROUTING_PROTOCOL_H
