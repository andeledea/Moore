#pragma once
#include <vector>
#include "fluke.h"
#include "voltmeter.h"

struct ambVals
{
	std::vector<double> t;  // the Fluke has multiple probes
	double p = 0;
	double u = 0;
};

class Paramb
{
public:
	/**
	* Initializes all the amb instruments
	* */
	void init();

	/**
	* Scans all the instruments
	*
	* @return currentParams: The parameters values
	* */
	ambVals scanParams();

	/**
	* Returns the current parameters 
	* without scanning the instruments
	*
	* @return currentParams: The parameters values
	* */
	ambVals getCurrentParams() { return currentParams; }

private:
	Fluke tempInstr;
	Voltmeter upInstr;

	ambVals currentParams;
};