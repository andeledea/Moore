#include "voltmeter.h"

void Voltmeter::connect()
{
	handle = ibdev(0, 22, 0, 11, 1, 0x140A);
	std::cout << "Voltmeter connected" << std::endl;
}

void Voltmeter::setParams()
{
}

double Voltmeter::readPressure()
{
	std::string req = "DCV"; //chiedo la distanza
	ibwrt(handle, req.c_str(), req.length());

	char ret[50];
	ibrd(handle, ret, 20); //leggo la distanza
	ret[Ibcnt()] = 0;
	return std::stod(ret);
}

double Voltmeter::readHumidity()
{
	std::string req = "DCV"; //chiedo la distanza
	ibwrt(handle, req.c_str(), req.length());

	char ret[50];
	ibrd(handle, ret, 20); //leggo la distanza
	ret[Ibcnt()] = 0;
	return std::stod(ret);
}

Voltmeter::~Voltmeter()
{
	handle = -1;
}
