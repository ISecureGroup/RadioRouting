#ifndef RADIOROUTING_SHOWME_H
#define RADIOROUTING_SHOWME_H

void            ShowEvent(char *s);
void 			PrintAddress(unsigned long address);
void            PrintPacket(Packet exmpl);
int             PrintPacketLine(Packet exmpl);
void            ShowRAMTable(WorkTable *ram);

#endif
