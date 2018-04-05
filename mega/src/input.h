#ifndef _INPUT_H_
#define _INPUT_H_

#include <Bounce2.h>

void setup_input();
void loop_input();

int16_t get_analog_value(uint8_t pin, int min = -1000, int max = 1000, int deadzone_low = 448, int deadzone_high = 574);
int16_t get_analog_value_slider(uint8_t pin, int min = 0, int max = 1000, int deadzone = 0);

#endif