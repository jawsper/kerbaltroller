#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "structs.h"
extern HandshakePacket handshake_packet;
extern ControlPacket control_packet;

void process_packets();

bool send_packet(uint8_t* data, uint8_t len);
#define send(packet) send_packet((uint8_t*)&packet, sizeof(packet))

#endif