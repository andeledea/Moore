#pragma once
#include <thread>
#include "Asse.h"
#include "CHRocodile.h"
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
	
	/**
	* Starts the measure with the confocal
	* objective with the given parameters
	* 
	* @param filename: the folder name that will contain the output
	* @param path: the location where the folder is created
	* @param speed: the speed of the measurement
	* @param track: if true the measuring axis will track the sample
	* @param start: the start relative position
	* @param stop: the stop relative position
	* */
	void measCHR(std::string nome_file, std::string path, int speed, bool track, long start, long stop);

	~Moore();

	SimpleSerial ser;

	Asse Xaxis, Yaxis, Zaxis;
	
	Laser las;
	IkOptical ik;
	Scale yScale, zScale;
	
	CHRocodile CHR;
	Cary cary;

private:
	pos zeroPos, abs, rel;
};
