#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#include "structs.h"

class COutput {
private:
	unsigned long stage_blink;
public:
	void setup();
	void set(const DisplayMode&, VesselData*);
	void update();
};

extern COutput Output;

#endif