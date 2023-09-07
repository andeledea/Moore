#include "paramb.h"

void Paramb::init()
{
#ifndef GUI
	tempInstr.connect();
	tempInstr.setParams();
#endif
}

ambVals Paramb::scanParams()
{
	ambVals vals;

	vals.t = tempInstr.readInstr();

	return vals;
}
