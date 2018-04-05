#ifndef _Packet_H_
#define _Packet_H_

#include <Arduino.h>

class CPacket {
private:
    uint8_t rx_len;
    uint8_t rx_packet_index;

public:
    int get(uint8_t* packet, uint8_t* packet_len);
};

extern CPacket Packet;

// --------------------

// enum class ActionGroups : uint16_t {
//     AGSAS      = 0x01, // 0
//     AGRCS      = 0x02, // 1
//     AGLight    = 0x04, // 2
//     AGGears    = 0x08, // 3
//     AGBrakes   = 0x10, // 4
//     AGAbort    = 0x20, // 5
//     AGCustom01 = 0x40, // 6
//     AGCustom02 = 0x80, // 7
//     AGCustom03 = 0x100, // 8
//     AGCustom04 = 0x200, // 9
//     AGCustom05 = 0x400, // 10
//     AGCustom06 = 0x800, // 11
//     AGCustom07 = 0x1000, // 12
//     AGCustom08 = 0x2000, // 13
//     AGCustom09 = 0x4000, // 14
//     AGCustom10 = 0x8000, // 15
// };

#endif