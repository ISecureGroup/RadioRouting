#ifndef RADIOROUTING_PAXAMPLE_H
#define RADIOROUTING_PAXAMPLE_H

//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload 			                                #   |
//      |     variable          		|	     |          |                            |                            |                |       |     |     |     |     |        |                           |                             |                 |        											#   |
//      |     variable          		|	     |          |                            |                            |                |       |     |     |     |     |        |                           |                             |                 |        											#	|
unsigned char PacketType0_1[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
unsigned char PacketType0_2[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x02 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
unsigned char PacketType0_3[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x03 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
unsigned char PacketType0_4[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x04 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType0_E[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x65 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x01 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
unsigned char PacketType0_E1[128] 	= 	{0x24 ,/**/ 0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x65 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType1_1[128] 	=  	{0x24 ,/**/ 0x01 ,/**/ 0x00 ,0x00 ,0x01 ,0x01 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x02 ,0x23};
unsigned char PacketType1_2[128] 	=  	{0x24 ,/**/ 0x01 ,/**/ 0x00 ,0x00 ,0x01 ,0x02 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x00, 0x00, 0x04,0x23};
unsigned char PacketType1_3[128] 	=  	{0x24 ,/**/ 0x01 ,/**/ 0x00 ,0x00 ,0x01 ,0x03 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x00 ,0x00, 0x00, 0x04 ,0x23};
unsigned char PacketType1_E[128] 	= 	{0x24 ,/**/ 0x01 ,/**/ 0x00 ,0x00 ,0x00 ,0x65 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x01 ,0x01 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x23	};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType2_0[128] 	= 	{0x24 ,/**/ 0x02 ,/**/ 0x01 ,0x01 ,0x01 ,0x01 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x69 ,0x69 ,0x69 ,0x69, 0x34 ,0x34 ,0x34 ,0x34, 0x23};
unsigned char PacketType2_1[128] 	= 	{0x24 ,/**/ 0x02 ,/**/ 0x01 ,0x01 ,0x01 ,0x02 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x34 ,0x34 ,0x34 ,0x34, 0x69 ,0x69 ,0x69 ,0x69, 0x23};
unsigned char PacketType2_2[128] 	= 	{0x24 ,/**/ 0x02 ,/**/ 0x01 ,0x01 ,0x01 ,0x03 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x34 ,0x34 ,0x34 ,0x11, 0x69 ,0x69 ,0x69 ,0x69, 0x23};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType3_1[128] 	= 	{0x24 ,/**/ 0x03 ,/**/ 0x00 ,0x00 ,0x00 ,0x04 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x69 ,0x34 ,0x21 ,0x21 ,0xff ,0xff, 0xff,0xff, 0x33 ,0x33 ,0x33 ,0x33 ,0x03 ,0x03 ,0x03 ,0x03 ,0x69 ,0x69 ,0x69 ,0x69 ,0x23};
unsigned char PacketType3_2[128] 	= 	{0x24 ,/**/ 0x03 ,/**/ 0x00 ,0x00 ,0x00 ,0x02 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x69 ,0x34 ,0x21 ,0x21 ,0xff ,0xff, 0xff,0xff, 0x33 ,0x33 ,0x33 ,0x33 ,0x03 ,0x03 ,0x03 ,0x03 ,0x69 ,0x69 ,0x69 ,0x69 ,0x23};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType4_1[128]   	= 	{0x24 ,/**/ 0x04 ,/**/ 0x34 ,0x21 ,0x21 ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x39 ,0x34 ,0x28 ,0x21 ,0xff ,0xff, 0xff, 0xff, 0x33 ,0x33 ,0x33 ,0x33 ,0x03 ,0x03 ,0x03 ,0x03 ,0x69 ,0x69 ,0x69 ,0x69 ,0x23};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType5_1[128] 	= 	{0x24 ,/**/ 0x05 ,/**/ 0x11 ,0x22 ,0x33 ,0x44 ,/**/ 0x69 ,0x69 ,0x69 ,0x69 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x69, 0x69 ,0x69 ,0x69 ,0x01 ,0x00 ,0x07 ,0xaa ,0xad ,0xef ,0x1a ,0x00 ,0x00 ,0x90 ,0x00 ,0x01 ,0x00 ,0x51 ,0x33 ,0x52 ,0x23}; 			;
unsigned char PacketType5_2[128]	= 	{0x24 ,/**/ 0x05 ,/**/ 0xaf ,0xff ,0xfa ,0x69 ,/**/ 0x69 ,0x69 ,0x69 ,0x69 ,/**/ 0x00 ,0x00 ,/**/ 0x90 ,0x00 ,0x01 ,0x00 ,0x01 ,0x55 ,/**/ 0x55 ,0x55 ,0x55 ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x55 ,/**/ 0x55 ,0x33 ,/**/ 0x11 ,0x66 ,0x12 ,0x43 ,0x0f ,0xf7 ,0xaa ,0xad ,0xef ,0x1a ,0xaa ,0x00 ,0x00 ,0x66 ,0x02 ,0x12 ,0x53 ,0x44 ,0x5b ,0x5b ,0x23};
//      |     variable          		|  start |   type   |       source address       |    destination address     |   sync time    | level |sess |sean.|stat |order|   TTL  |       next address        |        prev address         |     reserve     |Payload
unsigned char PacketType6_1[128]   	= 	{0x24 ,/**/ 0x06 ,/**/ 0x69 ,0x69 ,0x69 ,0x09 ,/**/ 0x69 ,0x69 ,0x69 ,0x69 ,/**/ 0x1a ,0x00 ,/**/ 0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x01 ,/**/ 0x55 ,0x55 ,0x55 ,0x55 ,/**/ 0x00 ,0x00 ,0x00 ,0x00 ,/**/ 0x55 ,0x55 ,/**/ 0x69, 0x69 ,0x69 ,0x69 ,0x01 ,0x00 ,0x07 ,0xaa ,0xad ,0xef ,0x1a ,0x00 ,0x00 ,0x90 ,0x00 ,0x01 ,0x00 ,0x51 ,0x33 ,0x52 ,0x33, 0x11 ,0x66 ,0x12 ,0x43 ,0x0f ,0xf7 ,0xaa ,0xad ,0xef ,0x1a ,0xaa ,0x00 ,0x00 ,0x66 ,0x02 ,0x12 ,0x53 ,0x44 ,0x5b ,0x23};
//ERROR PACKET//////////////////
unsigned char PacketTypeZERO[128]   =   {0};
//SENSORS///////////////////////
unsigned char SensorData_0[20]   	=  {0x23};
unsigned char SensorData_1[20]   	=  {0x22, 0x11 ,0x00 ,0x00 ,0x01 ,0x00 ,0x07 ,0xaa ,0xad ,0xef ,0x1a ,0x00 ,0x00 ,0x90 ,0x00 ,0x01 ,0x00 ,0x51 ,0x33 ,0x52};
unsigned char SensorData_2[20]   	=  {0x33, 0x11 ,0x66 ,0x12 ,0x43 ,0x0f ,0xf7 ,0xaa ,0xad ,0xef ,0x1a ,0xaa ,0x00 ,0x00 ,0x66 ,0x02 ,0x12 ,0x53 ,0x44 ,0x5b};


#endif
