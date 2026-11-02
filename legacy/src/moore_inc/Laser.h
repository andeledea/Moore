#pragma once

#include "ni4882.h"
#include "PosInstr.h"

/**
* Interface for the red laser interferometer
* */
class Laser : PosInstr
{
public:
	using PosInstr::PosInstr;

	/**
	* Connects the GPIB laser interface
	* */
	void connect();

	/**
	* Sets the refraction coefficient of the laser
	* */
	void setParams();

	/**
	* Reads the laser interferometer value
	* 
	* @return value: the position of the mirror (mm)
	* */
	double readInstr();

	~Laser();
};

