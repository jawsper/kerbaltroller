#include "structs.h"
#include "input.h"
#include "communication.h"


void setup() {
  Serial.begin(38400);
  Serial3.begin(9600);

  setup_input();
  Serial3.write(0xFE);
  Serial3.write(0x01);
  // Serial3.print("Kerbaltroller v1");
}


void loop() {
  loop_input();

  process_packets();

}
