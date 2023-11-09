#ifndef KEYSIGHT_H
#define KEYSIGHT_H

#include "PosInstr.h"

/**
* Interface for the red laser interferometer
* */
class KeySight : PosInstr
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

	~KeySight();
};

// https://github.com/blctrl/1735/blob/master/blcApp/src/blc1735.cpp

#endif

