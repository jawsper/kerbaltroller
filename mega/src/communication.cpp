#include <Arduino.h>
#include "communication.h"
#include "Packet.h"
#include "structs.h"
#include "display.h"
#include "output.h"

HandshakePacket handshake_packet;
ControlPacket control_packet;


void process_packets() {

  static uint8_t buffer[256];
  static uint8_t packet_len;

  int packet_result = Packet.get((uint8_t*)&buffer, &packet_len);
  if(packet_result == 0) {
    if(packet_len > 0) {
      int packet_id = buffer[0];

      switch(packet_id) {
        case 0: // HANDSHAKE
          send(handshake_packet);
          Serial3.write(0xFE);
          Serial3.write(0x01); // clear
          break;
        case 1: // VESSEL_DATA
          // Serial3.write(0xFE);Serial3.write(128);
          // Serial3.write("VESSEL");
          Output.set(DisplayMode::Orbit, (VesselData*)&buffer);
          Display.display_vesseldata(DisplayMode::Orbit, (VesselData*)&buffer);
          break;
      }

    }
  }
}

bool send_packet(uint8_t* data, uint8_t len) {
  uint8_t old_sreg = SREG;
  cli();
  Serial.write(0xBE);
  Serial.write(0xEF);
  Serial.write(len);
  uint8_t checksum = len;
  for(uint8_t i = 0; i < len; i++) {
    checksum ^= data[i];
    Serial.write(data[i]);
  }
  Serial.write(checksum);
  SREG = old_sreg;

  return true;
}