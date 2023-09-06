#pragma once

#include "SimpleSerial.h"
#include "PosInstr.h"

/**
* Interface for the red laser interferometer
* */
class Cary : PosInstr
{
public:
	using PosInstr::PosInstr;

	/**
	* Connects the CARY interface
	* The connection is serial RS232 optoisolated.
	* The connection provides the correct signals for the
	* photodiode (Dtr, Rts controls).
	* */
	void connect();

	/**
	* Sets the parameters of the cary
	* */
	void setParams();

	/**
	* Reads the cary value
	* 
	* @return value: the displacement of the CARY (mm)
	* */
	double readInstr();

	~Cary();

private:
	SimpleSerial ser;
};