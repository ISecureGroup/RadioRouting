/**
 * Данный файл содержит точку входа в приложение, которое управляет неким устройством на базе микроконтроллера
 * (далее Устройство). Устройство в рамках своей работы обменивается данными с другими устройствами через беспроводную
 * сенсорную сеть, построенную в соответсвии с _Протоколом_. Логика работы _Протокола_ описана в функции #PacketManager().
 * Приём пакетов из сети и передача пакетов в сеть происходит через радиомодуль, с которым микроконтроллер общается через
 * последовательный порт (COM-порт, serial-port, RS-232). Получение пакета и отправка пакета в виде неструктурированных
 * данных (char-массивов) производится с помощью функций, работающих с COM-портом #Когда будет готова функция надо вписать
 * Обработка пакета (структурирование, реакция на полученное с пакетом сообщение, формирование ответного пакета),
 * производится функицей #PacketManager().
 *
 * @authors Kutuzov V.I (kutuzovvladislav@mail.ru), Azizov R.R (leofoxi73@gmail.com), Elyagin S.V. (esv@ulstu.ru)
 * @version 1.0
 */

#include <time.h>
#include <windows.h>
#include "protocol/protocol.h"
#include "../tests/paxample.h"
#include "../tests/showme.h"

WorkTable       RAM;                                        //Память, выделяемая под логику протокола
int             RSSI;                                       //Значение урвня принятого сигнала
/**
 * Функция выполняющаяся при запуске микроконтроллера. Служит для первоначальной инициализации переменных.
 */
unsigned char OutPacket[128];
unsigned char InPacket[128];

void setup(){
    StartInitProtocol(&RAM);
}
/**
 * Функция, в которой сосредоточена основная логика работы устройства. Данная функция выполняется в бесконечном цикле,
 * пока на устройство подаётся питание.
 */
void loop()
{
    //Serial.readByteUntill()
    if(PacketManager(SensorData_0, 200, &RAM, PacketType0_1,OutPacket))
    {
        //Serial.write(OutPacket)
    }

/**        // В SLEEP
        ShowRAMTable(&RAM);
        PacketManager(SensorData_0, 200, &RAM, PacketType0_1,OutPacket);
        PacketManager(SensorData_0, 300, &RAM, PacketType0_2,OutPacket);
        PacketManager(SensorData_0, 200, &RAM, PacketType0_3,OutPacket);
        PacketManager(SensorData_0, 100, &RAM, PacketType0_4,OutPacket);
        ShowRAMTable(&RAM);
        Sleep(7000);
        // SEND_01
        PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO,OutPacket);
        // START_DEF_ROUTERS
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_1,OutPacket);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_2,OutPacket);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType1_3,OutPacket);
        ShowRAMTable(&RAM);
        Sleep(7000);
        //SEND 02
        PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO,OutPacket);
        //WAIT CONFIRM FROM POTENTIAL ROUTER
        PacketManager(SensorData_0, RSSI, &RAM, PacketType3_1,OutPacket);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType3_2,OutPacket);
        ShowRAMTable(&RAM);
        Sleep(7000);
        //ANNOUNCEMENT POTENTIAL ROUTER STATUS
        //SEND 00
        PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO,OutPacket);
        ShowRAMTable(&RAM);
        Sleep(7000);
        //WAITING CONFIRM ROUTER STATUS FROM DEVICES
        PacketManager(SensorData_0, RSSI, &RAM, PacketType2_0,OutPacket);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType2_1,OutPacket);
        PacketManager(SensorData_0, RSSI, &RAM, PacketType2_2,OutPacket);
        ShowRAMTable(&RAM);
        Sleep(7000);
        //SEND 03
        PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO,OutPacket);
        ShowRAMTable(&RAM);
        //ADDITIONAL WAITING CONFIRM ROUTER STATUS FROM DEVICES
        ShowRAMTable(&RAM);
        Sleep(7000);
        //SEND 03
        PacketManager(SensorData_0, RSSI, &RAM, PacketTypeZERO,OutPacket);
        ShowRAMTable(&RAM);
        //READY
        Sleep(7000);
        ShowRAMTable(&RAM);
        Sleep(555555);
*/
}
/**
 * Функция, которая служит точкой входа в приложение.
 */
int  main() {
    system("chcp 65001");
    setup();
    while(1)
       loop();
}