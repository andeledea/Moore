#pragma once
#include <thread>
#include "Asse.h"
#include "CHRocodile.h"
#include "IkOptical.h"
#include "Scale.h"
#include "SimpleSerial.h"
#include "pos.h"

class Moore
{
public:
	Moore() {};
	void defineZero();
	
	void updatePosition();
	pos getAbsPosition() { return abs; };
	pos getRelPosition() { return rel; };

	void setAbsPosition(pos target);
	void setRelPosition(pos target);
	
	void moveInstr();
	void measCHR(std::string nome_file, std::string path, int speed, bool track, long start, long stop);

	void init();

	SimpleSerial ser;

	Asse Xaxis, Yaxis, Zaxis;
	
	Laser las;
	IkOptical ik;
	Scale yScale, zScale;
	
	CHRocodile CHR;

private:
	pos zeroPos, abs, rel;
};
