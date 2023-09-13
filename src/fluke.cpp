#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "fluke.h"

void Fluke::connect()
{
	std::string addr = "192.168.170.100";
	int port = 3490;

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
	auto send = [this](char const* cmd)
	{
		this->client.send_data(cmd);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};
	
	// Setting for a single scan of all temps
	send("*RST\n");
	send("CONF:TEMP FRTD, ABC, (@201:204)\n");
	send("ROUT:SCAN (@201:204)\n");
	// ser.WriteSerialPort("TRIG:COUN 0");  // used for continous scanning
}

std::vector<double> Fluke::readAllInstr()
{
	std::cout << "Reading Fluke" << std::endl;
	auto sendRec = [this](char const* cmd)
	{
		std::string rec;
		this->client.send_data(cmd);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		rec = client.receive();
		std::cout << "Fluke received: " << rec << std::endl;
		return rec;
	};
	
	client.send_data("INIT\n");
	sendRec("STAT:OPER?\n");  // flush the queue
	
	int r = 0;
	while (r != 272) { r = std::stoi(sendRec("STAT:OPER?\n")); };
	
	std::string vals = sendRec("FETC?\n");
	std::vector<double> vals_d (Fluke::N_ACTIVE_CH, 0.0);  // out vec with all zeros
	
	std::string::size_type sz;
	for (int i = 0; i < Fluke::N_ACTIVE_CH; i++)  // parse the whole response
	{
		if (i == 0) vals_d[i] = std::stod(vals, &sz);
		else vals_d[i] = std::stod(vals.substr(sz));
	}
	return vals_d;
}

Fluke::~Fluke()
{

}
