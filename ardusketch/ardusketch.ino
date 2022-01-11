#include "protocol.hh"

WorkTable       RAM;                                                //ВЫДЕЛЕНИЕ ПАМЯТИ ДЛЯ ДАННЫХ
int             RSSI;                                               //ПРОГРАМНЫЙ RSSI 
unsigned char   serial_buffer[128];                                 //БУФФЕР ДЛЯ ДАННЫХ ПРИНЯТЫХ С СЕРИЙНИКА

void setup() {
    RAM.MAC = 0x69696969;                                           // Мак адрес устройства, в дальнейшем будет случайным
    RAM.Status = SLEEP;                                             // Первоначальное состояние устройства
    RAM.start_status_time = millis();                               // Сохраняем время со старта
}

void loop() 
{
  if(Serial.available() > 0){
      int i=0;
      while(Serial.available() > 0){
          serial_buffer[i] = Serial.read();
          i++;
      }
      PacketManager(0,RSSI,&RAM,serial_buffer);
  }
  else{
      PacketManager(0,RSSI,&RAM,0);
  }

}
