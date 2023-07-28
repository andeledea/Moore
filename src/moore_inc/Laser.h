#pragma once

#include "ni4882.h"
#include "PosInstr.h"

class Laser : PosInstr
{
public:
	using PosInstr::PosInstr; // eredito i costruttori

	void connect();
	void setParams(); //set the refraction coeff
	double readInstr();

	~Laser();
};

