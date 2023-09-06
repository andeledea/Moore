#include <iostream>
#include "cary.h"

void Cary::connect()
{
	DCB dcbSerialParams = { 0 };

	dcbSerialParams.BaudRate = CBR_4800;
	dcbSerialParams.ByteSize = 7;
	dcbSerialParams.StopBits = 2;
	dcbSerialParams.Parity = EVENPARITY;

	// Set the lines for the photodiode
	dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

	char port[] = "COM2";
	if (ser.OpenSerialPort(port, dcbSerialParams)) // apro la com seriale
	{
		std::cout << "Cary connesso" << std::endl;
	}
	else
	{
		std::cout << "Cary NON connesso" << std::endl;
		throw std::runtime_error("Unable to connect CARY");
	}
}

void Cary::setParams()
{
	// reference commands at: 
	// https://www.whp.cz/files/User%20manual,%20TESA%20TT80-90%20EN%20DE%20FR.pdf#page=94
	std::string rec;
	
	ser.WriteSerialPort("ID?\r");
	rec = ser.ReadSerialPort(1);
	std::cout << "Cary ID: " << rec << std::endl;

	this->max = 0.1;
	this->min = -0.1;
}

double Cary::readInstr()
{
	/**
	* Data transfer
	* TT80and TT90 in TT80 mode TT90and TT90 - UPC
	* Mm: ±xxxx.xx <cr / lf> µm : ±xxx.xxx <cr / lf>
	* In : ±.xxxxxx <cr / lf> mil : ±(x)x.xxxx(x) < cr / lf >
	* */
	char toSend[] = "?\r"; // Request for displayed value.
	ser.WriteSerialPort(toSend);
	
	std::string rec = ser.ReadSerialPort(1);
	std::cout << "Cary read: " << rec << std::endl;
	return 0.0;
}

Cary::~Cary()
{
	ser.CloseSerialPort();
}
