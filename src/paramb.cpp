#include "paramb.h"

void Paramb::init()
{
#ifndef GUI
	// INIT THE TEMP INSTRUMENT
	tempInstr.connect();
	tempInstr.setParams();

	// INIT THE VOLTMETER
	// upInstr.connect();
	// upInstr.setParams();
#endif
}

ambVals Paramb::scanParams()
{
	ambVals vals;

	vals.t = tempInstr.readAllInstr();
	// vals.p = upInstr.readPressure();
	// vals.u = upInstr.readHumidity();

	return vals;
}
