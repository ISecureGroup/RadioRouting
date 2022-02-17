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

#include "protocol.hpp"

unsigned char OutPacket[128];
unsigned char InPacket[128];
unsigned char SensorData[128];
WorkTable       RAM;                                        //Память, выделяемая под логику протокола
int             RSSI;                                       //Значение урвня принятого сигнала
/**
 * Функция выполняющаяся при запуске микроконтроллера. Служит для первоначальной инициализации переменных.
 */
void setup(){
    StartInitProtocol(&RAM);
}
/**
 * Функция, в которой сосредоточена основная логика работы устройства. Данная функция выполняется в бесконечном цикле,
 * пока на устройство подаётся питание.
 */
void loop() {
  PacketManager(SensorData, 200, &RAM, InPacket,OutPacket);

}