#include <Time.h>
#include <stdlib.h>
#include "protocol.hh"

//////////////////////ВЫДЕЛЯЕТСЯ ВСЯ ВНУТРЕННЯЯ ПАМЯТЬ И RSSI ПРИ ОТСУТСТВИИ АППАРАТНОГО
WorkTable       RAM;
int             RSSI;
unsigned char   serial_buffer[128];

void setup() {
    RAM.MAC = 0x69696969;                                                                                               // Мак адрес устройства, в дальнейшем будет случайным
    RAM.Status = SLEEP;                                                                                                 // Первоначальное состояние устройства
    RAM.start_status_time = millis();
}

void loop() {
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
