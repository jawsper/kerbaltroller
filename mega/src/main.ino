#include "structs.h"
#include "input.h"
#include "communication.h"
#include "output.h"
#include "display.h"


void setup() {
  Serial.begin(38400);

  Display.setup();
  Input.setup();
  Output.setup();
}


void loop() {
  Input.loop();

  process_packets();

  Output.update();
}
