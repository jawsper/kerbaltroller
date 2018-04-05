#include <Arduino.h>
#include "input.h"
#include "structs.h"
#include "communication.h"

// btn, led
const InputOutput buttons[] = {
  { 1, 25, 24, true }, // [0] brakes
  { 1, 27, 26, true }, // [1] lights
  { 1, 31, 30, true }, // [2] gear
  { 1, 33, 32, true }, // [3] sas
  { 1, 37, 36, true }, // [4] rcs
  { 1, 39, 38, true }, // [5] stage
  { 1, 43, 42, true }, 
  { 1, 45, 44, true },
  { 2, 50, 51, false }, // [8] danger 1 (stage lock)
  { 2, 48, 49, false }, // [9] danger 2 (abort)
};

#define SWITCH_COUNT (sizeof(buttons) / sizeof(buttons[0]))

const Input inputs[] = {
    { InputType::Brakes, 1, 25, true, MainControls::Brakes, InputType::None },
    { InputType::Lights, 1, 27, true, MainControls::Lights, InputType::None },
    { InputType::Gears, 1, 31, true, MainControls::Gears, InputType::None },
    { InputType::SAS, 1, 33, true, MainControls::SAS, InputType::None },
    { InputType::RCS, 1, 37, true, MainControls::RCS, InputType::None },
    { InputType::Stage, 1, 39, true, MainControls::Stage, InputType::StageLock },
    { InputType::None, 1, 43, true, MainControls::None, InputType::None }, // unused
    { InputType::None, 1, 45, true, MainControls::None, InputType::None }, // unused
    { InputType::StageLock, 2, 50, false, MainControls::None, InputType::None }, // stage lock
    { InputType::Abort, 2, 48, false, MainControls::Abort, InputType::None },
};

#define INPUT_COUNT (sizeof(inputs) / sizeof(Input))


#define BTN_BRAKES 0
#define BTN_LIGHTS 1
#define BTN_GEAR 2
#define BTN_SAS 3
#define BTN_RCS 4
#define BTN_STAGE 5
#define BTN_STAGE_LOCK 8
#define BTN_ABORT 9

const ControlToButton controls_to_buttons[] = {
  { MainControls::Stage, BTN_STAGE, BTN_STAGE_LOCK },
  { MainControls::Brakes, BTN_BRAKES, -1 },
  { MainControls::Lights, BTN_LIGHTS, -1 },
  { MainControls::Gears, BTN_GEAR, -1 },
  { MainControls::Abort, BTN_ABORT, -1 },
  { MainControls::RCS, BTN_SAS, -1 },
  { MainControls::SAS, BTN_RCS, -1 },
};

#define CONTROLS_TO_BUTTONS_COUNT (sizeof(controls_to_buttons) / sizeof(controls_to_buttons[0]))

Bounce debouncers[SWITCH_COUNT];

#define ANALOG_PITCH A1
#define ANALOG_YAW A0
#define ANALOG_ROLL A2
#define ANALOG_THROTTLE A3

void setup_input() {
  uint8_t i;
  for(i = 0; i < SWITCH_COUNT; i++) {
    switch(buttons[i].type) {
      case 1:
        pinMode(buttons[i].input, INPUT_PULLUP);
        pinMode(buttons[i].output, OUTPUT);
        digitalWrite(buttons[i].output, LOW);
        break;
      case 2:
        pinMode(buttons[i].input, INPUT);
        pinMode(buttons[i].output, OUTPUT);
        digitalWrite(buttons[i].output, HIGH);
        break;
    }
    debouncers[i].attach(buttons[i].input);
    debouncers[i].interval(5);
  }
}

bool get_button_val(int button) {
  bool val = debouncers[button].read();
  if(buttons[button].invert) val = !val;
  return val;
}

unsigned long last_update = 0;
const unsigned long update_interval = 10;

void loop_input()  {
  uint8_t i;
  for(i = 0; i < SWITCH_COUNT; i++) {
    debouncers[i].update();
  }

  if((millis() - last_update) >= update_interval) {
    last_update = millis();

    for(i = 0; i < CONTROLS_TO_BUTTONS_COUNT; i++) {
      bool val = get_button_val(controls_to_buttons[i].button);
      if(controls_to_buttons[i].lock >= 0) {
        if(!get_button_val(controls_to_buttons[i].lock)) val = false;
      }

      control_packet.set_main_control((uint8_t)controls_to_buttons[i].control, val);
    }

    control_packet.pitch = get_analog_value(ANALOG_PITCH);
    control_packet.roll = get_analog_value(ANALOG_ROLL);
    control_packet.yaw = get_analog_value(ANALOG_YAW);
    control_packet.throttle = get_analog_value_slider(ANALOG_THROTTLE);
    send(control_packet);
  }
}
int16_t get_analog_value(uint8_t pin, int min, int max, int deadzone_low, int deadzone_high) {
  int16_t value = analogRead(pin);
  int16_t center = max - ((max - min) / 2);

  if(value >= deadzone_high) return map(value, deadzone_high, 1023, center, max);
  if(value <= deadzone_low) return map(value, 0, deadzone_low, min, center);
  return center;
}

int16_t get_analog_value_slider(uint8_t pin, int min, int max, int deadzone) {
  int16_t value = analogRead(pin);
  if(value < deadzone) return min;
  if(value > (1023 - deadzone)) return max;
  return map(value, deadzone, 1023 - deadzone, min, max);
}