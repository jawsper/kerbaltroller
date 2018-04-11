#include <Arduino.h>
#include "structs.h"
#include "output.h"

const OutputPin outputs[] = {
	{ 24, OutputType::DEFAULT_LOW, 24 }, // brakes
	{ 26, OutputType::DEFAULT_LOW, 26 }, // lights
	{ 30, OutputType::DEFAULT_LOW, 30 }, // gear
	{ 32, OutputType::DEFAULT_LOW, 32 }, // SAS
	{ 36, OutputType::DEFAULT_LOW, 36 }, // RCS
	{ 38, OutputType::DEFAULT_LOW, 38 }, // stage
	{ 42, OutputType::DEFAULT_LOW, 42 },
	{ 44, OutputType::DEFAULT_LOW, 44 },
	{ 51, OutputType::DEFAULT_HIGH, 51 }, // danger stage lock
	{ 49, OutputType::DEFAULT_HIGH, 49 }, // danger abort
};

#define OUTPUT_COUNT (sizeof(outputs) / sizeof(OutputPin))

void COutput::setup() {
	for(uint8_t i = 0; i < OUTPUT_COUNT; i++) {
		pinMode(outputs[i].pin, OUTPUT);

		switch(outputs[i].type) {
			case OutputType::DEFAULT_LOW:
				digitalWrite(outputs[i].pin, HIGH);
				delay(50);
				digitalWrite(outputs[i].pin, LOW);
				break;
			case OutputType::DEFAULT_HIGH:
				digitalWrite(outputs[i].pin, HIGH);
				break;
		}
	}
	stage_blink = 0;
}

#define SET_OUTPUT(i, field) digitalWrite(outputs[i].pin, vessel_data->action_groups.field ? HIGH : LOW)

void COutput::set(const DisplayMode& mode, VesselData* vessel_data) {
	SET_OUTPUT(0, brakes_on);
	SET_OUTPUT(1, lights_on);
	SET_OUTPUT(2, gears_on);
	SET_OUTPUT(3, sas_on);
	SET_OUTPUT(4, rcs_on);
	// SET_OUTPUT(5, brakes_on);

}

void COutput::update() {
	if(digitalRead(50)) {
		if(stage_blink == 0) {
			stage_blink = millis();
			digitalWrite(outputs[5].pin, HIGH);
		} else {
			if((millis() - stage_blink) > 750) {
				digitalWrite(outputs[5].pin, !digitalRead(outputs[5].pin));
				stage_blink = millis();
			}
		}
	} else {
		digitalWrite(outputs[5].pin, LOW);
	}
}

COutput Output;