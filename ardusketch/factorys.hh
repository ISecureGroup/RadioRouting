
#ifndef FACTORYS_HH
#define FACTORYS_HH
#include "protocol.hh"

void            packetConstructor(WorkTable *ram, unsigned char   _startpacket, unsigned char	_typepacket, unsigned long	_sourceaddres, unsigned long	_destinationaddres, unsigned short	_synctime, unsigned char	_session, unsigned char	_level, unsigned char	_seance, unsigned char	_nodestate, unsigned char	_ordernumder, unsigned char	_ttl, unsigned long 	_nextaddres, unsigned long 	_prevaddres, unsigned short 	_reserve, unsigned char	*_payload);
//------------------------FACTORY--------------------------
void            packet_Factory_00(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_01(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я ПОТЕНЦИАЛЬНЫЙ РОУТЕР"
void            packet_Factory_02(WorkTable * ram);                                                                       	//ФАБРИКА ПАКЕТА "Я ВЫБРАЛ РОУТЕР"
void            packet_Factory_03(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "Я РОУТЕР"
void            packet_Factory_04(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОПРОС УСТРОЙСТВ"
void            packet_Factory_05(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА УНО"
void            packet_Factory_06(WorkTable * ram);                                                                         //ФАБРИКА ПАКЕТА "ОТВЕТ ОТ УСТРОЙСТВА МЕНИ"

#endif //FACTORYS_HH
