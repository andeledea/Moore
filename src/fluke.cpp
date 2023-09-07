#include <iostream>
#include "fluke.h"

void Fluke::connect()
{
	char port[] = "COM4";
	if (ser.OpenSerialPort(port, CBR_14400)) // apro la com seriale
	{
		std::cout << "Fluke connected" << std::endl;
	}
	else
	{
		std::cout << "Fluke NOT connected" << std::endl;
		throw std::runtime_error("Unable to connect FLUKE");
	}
}

void Fluke::setParams()
{
	// Setting for a single scan of all temps
	ser.WriteSerialPort("*RST");
	ser.WriteSerialPort("TEMP:THER:TYPE K, (@101, 106:108)");
	ser.WriteSerialPort("ROUT:SCAN (@201:204)");
	// ser.WriteSerialPort("TRIG:COUN 0");  // used for continous scanning
}

double Fluke::readInstr()
{
	auto sendRec = [this](char const* cmd)
	{
		std::string rec;
		this->ser.WriteSerialPort(cmd);
		rec = ser.ReadSerialPort(1);
		return rec;
	};

	ser.WriteSerialPort("INIT");
	while (sendRec("STAT:OPER?") != "") {};
	
	std::string vals = sendRec("FETCH?");
	std::cout << "FLUKE: " << vals << std::endl;

	return 0.0;
}

Fluke::~Fluke()
{
	ser.CloseSerialPort();
}
