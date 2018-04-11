#include <Arduino.h>
#include "structs.h"
#include "display.h"


void CDisplay::setup() {
	Serial3.begin(9600);

	// turn display on and hide cursor
	write(0xFE);
	write(0x0C);

	// clear display
	write(0xFE);
	write(0x01);

	print(F("Kerbaltroller v1"));
	delay(500);
	
	// clear display
	write(0xFE);
	write(0x01);
}

size_t CDisplay::print_distance(float value, uint8_t digits=1) {
	const String units[] = {
		"m", "km", "Mm", "Gm"
	};
	const int number_of_units = 4;

	uint8_t unit = 0;
	while((value <= -1000. || value >= 1000.) && unit < number_of_units) {
		value /= 1000.;
		unit++;
	}

	return print(value, digits) + print(units[unit]);
}

size_t CDisplay::print_time(int32_t value) {
	if(abs(value) < 60)
		return print(value) + write('s');
	value /= 60;
	if(abs(value) < 60)
		return print(value) + write('m');
	value /= 60;
	if(abs(value) < 24)
		return print(value) + write('h');
	value /= 24;
	if(abs(value) < 365)
		return print(value) + write('d');
	value /= 365;
	return print(value) + write('y');
}


void CDisplay::display_vesseldata(const DisplayMode& mode, VesselData* vessel_data) {
	switch(mode) {
		case DisplayMode::None:
			break;
		case DisplayMode::Orbit:
			goto_line(0);
			print(F("AP "));
			print_distance(vessel_data->AP, 0);
			write(' ');
			print_time(vessel_data->TAp);
			fill();

			goto_line(1);
			print(F("PE "));
			print_distance(vessel_data->PE, 0);
			write(' ');
			print_time(vessel_data->TPe);
			fill();
			break;
	}
}

CDisplay Display;
