#include <iostream>
#include "fluke.h"

void Fluke::connect()
{
	std::string addr = "192.168.001.100";
	int port = 3980;

	if (client.conn(addr, port)) // apro la com seriale
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
	client.send_data("*RST\n");
	client.send_data("TEMP:THER:TYPE K, (@101, 106:108)\n");
	client.send_data("ROUT:SCAN (@201:204)\n");
	// ser.WriteSerialPort("TRIG:COUN 0");  // used for continous scanning
}

double Fluke::readInstr()
{
	auto sendRec = [this](char const* cmd)
	{
		std::string rec;
		this->client.send_data(cmd);
		rec = client.receive();
		std::cout << "Fluke received: " << rec << std::endl;
		return rec;
	};

	client.send_data("INIT");
	while (sendRec("STAT:OPER?") != "") {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	};
	
	std::string vals = sendRec("FETCH?");

	return 0.0;
}

Fluke::~Fluke()
{

}
