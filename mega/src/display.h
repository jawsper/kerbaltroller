#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "structs.h"

void setup_display();
void display_vesseldata(VesselData*);

class CDisplay : public Print {
public:
	void setup();
	void display_vesseldata(const DisplayMode&, VesselData*);

private:
	size_t print_distance(float value, uint8_t digits=1);
	size_t print_time(int32_t value);

	uint8_t current_line = 0;
	uint8_t current_line_count = 0;

	void goto_line(uint8_t line) {
		write(0xFE);
		write(line == 0 ? 128 : 192);
		current_line = line;
		current_line_count = 0;
	}

	void fill() {
		uint8_t count = 16 - current_line_count;
		while(count--) write(' ');
	}

	size_t write(uint8_t c) {
		size_t count = Serial3.write(c);
		current_line_count += count;
		return count;
	}
};

extern CDisplay Display;

#endif