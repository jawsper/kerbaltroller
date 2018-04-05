#include "Packet.h"

int CPacket::get(uint8_t* packet, uint8_t* packet_len) {
    if((rx_len == 0) && Serial.available() > 3) {
        while(Serial.read() != 0xBE) {
            if(Serial.available() == 0)
                return -1;
        }
        if(Serial.read() == 0xEF) {
            rx_len = Serial.read();
            packet[0] = Serial.read();
            rx_packet_index = 1;
        }
    }

    if(rx_len != 0) {
        while(Serial.available() && rx_packet_index <= rx_len) {
            packet[rx_packet_index++] = Serial.read();
        }

        if(rx_len == (rx_packet_index - 1)) {
            uint8_t calc_checksum = rx_len;
            for(uint8_t i = 0; i < rx_len; i++) {
                calc_checksum ^= packet[i];
            }

            if(calc_checksum == packet[rx_packet_index - 1]) {
                *packet_len = rx_len;
                rx_len = 0;
                return 0;
            }

            rx_len = 0;
            return -2;
        }
    }
    return -1;
}

CPacket Packet;