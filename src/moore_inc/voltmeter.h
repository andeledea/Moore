#pragma once

#include <string>
#include <iostream>
#include "ni4882.h"
#include "AmbInstr.h"

/**
* Class for ambiental parameters with old moore setup
* When the intruments are renewed split the classes in
* barometer and igrometer
* */
class Voltmeter : AmbInstr
{
	using AmbInstr::AmbInstr;

	/**
	* Connects the voltmeter using the GPIB interface
	* */
	void connect();

	/**
	* Sets the parameters of the instrument
	* */
	// https://xdevs.com/doc/HP_Agilent_Keysight/3458A/EPSG084364.pdf
	void setParams();

	/**
	* Reads the ambient pressure value
	* */
	double readPressure();

	/**
	* Reads the ambient humidity value
	* */
	double readHumidity();

	~Voltmeter();

private:
	int handle = -1;
};