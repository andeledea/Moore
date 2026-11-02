#include "Laser.h"
#include <iostream>
#include <string>

void Laser::connect() {
	handle = ibdev(0, 3, 0, 11, 1, 0x140A);
	std::cout << "[INFO] Laser connected" << std::endl;
}

double Laser::readInstr()
{
	std::string req = "XPOS?"; //chiedo la distanza
	ibwrt(handle, req.c_str(), req.length());
	
	char ret[50];
	ibrd(handle, ret, 20); //leggo la distanza
	ret[Ibcnt()] = 0;
	return atof(ret);
}

void Laser::setParams()
{
	std::string rifra = "XTCN0.9997287"; // lunga 13
	ibwrt(handle, rifra.c_str(), rifra.length());
}

Laser::~Laser()
{
	handle = -1;
}
