#include <Arduino.h>
#include "communication.h"
#include "Packet.h"
#include "structs.h"

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
          VesselData* vessel_data = (VesselData*)&buffer;          
          Serial3.write(0xFE);
          Serial3.write(128);

          Serial3.print("AP: ");
          Serial3.print(vessel_data->AP);
          Serial3.write(0xFE);
          Serial3.write(192);
          Serial3.print("PE: ");
          Serial3.print(vessel_data->PE);

          break;
      }

    }
  }
}

bool send_packet(uint8_t* data, uint8_t len) {
  Serial.write(0xBE);
  Serial.write(0xEF);
  Serial.write(len);
  uint8_t checksum = len;
  for(uint8_t i = 0; i < len; i++) {
    checksum ^= data[i];
    Serial.write(data[i]);
  }
  Serial.write(checksum);

  return true;
}