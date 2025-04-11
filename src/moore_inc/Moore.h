#pragma once
#include <thread>

#include "Laser.h"
#include "keysight.h"
#include "Asse.h"
#include "IkOptical.h"
#include "Scale.h"
#include "SimpleSerial.h"
#include "pos.h"
#include "cary.h"

/**
* Class control for the Moore machine
* */
class Moore
{
public:
	Moore() {};

	/**
	* Initializes all the machine instruments and
	* controls
	* */
	void init();

	/**
	* Define the zero position of the machine
	* and store it to set a center position 
	* for relative movement 
	* */
	void defineZero();
	
	/**
	* Reads the position from the encoders and
	* set the abs and relative variables to match
	* the physical position of the machine
	* */
	void updatePosition();

	/**
	* Returns the absolute position
	* 
	* @return abs: the absolute position
	* */
	pos getAbsPosition() { return abs; };

	pos getAbsPositionWithInstr();

	/**
	* Returns the relative position
	* if zero has not been defined returns
	* the absolute position
	* 
	* @return rel: the relative position
	* */
	pos getRelPosition() { return rel; };

	/**
	* Moves the machine to reach the target
	* position in Abs coordinates
	* 
	* @param target: the target 3D coords
	* */
	void setAbsPosition(pos target);
	
	/**
	* Moves the machine to reach the target
	* position in Rel coordinates
	* 
	* @param target: the target 3D coords
	* */
	void setRelPosition(pos target);

	/**
	* Moves the machine to reach the target
	* position (considering the axis measuring
	* instrument position)
	* 
	* @param target: the target 1D coord
	* */
	void moveInstr(double target);

	void keyboardMove(bool z_contr = false);
	
	~Moore();

	SimpleSerial ser;

	Asse Xaxis, Yaxis, Zaxis;
	
	Laser las;
	Keysight key;
	IkOptical ik;
	Scale yScale, zScale;
	Cary cary;

private:
	pos zeroPos, abs, rel;
};
