#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include "fluke.h"

void Fluke::connect()
{
	std::string addr = "192.168.170.100";
	int port = 3490;

	if (client.conn(addr, port)) // apro la com seriale
	{
		std::cout << "[INFO] Fluke connected" << std::endl;
	}
	else
	{
		std::cout << "[ERROR] Fluke not connected" << std::endl;
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
	send("*RCL 04\n");
	send("DATA:LOG:DEST MEM\n");
	send("DATA:LOG:AUTO OFF\n");
	send("TRIG:COUN 1\n");
	send("ROUT:SCAN (@201:204)\n");
}

std::vector<double> Fluke::readAllInstr()
{
	std::cout << "[INFO] Reading Fluke" << std::endl;
	auto sendRec = [this](char const* cmd)
	{
		std::string rec;
		this->client.send_data(cmd);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		rec = client.receive();
		std::cout << "[INFO] Fluke received: " << rec << std::endl;
		return rec;
	};
	
	client.send_data("INIT\n");
	sendRec("STAT:OPER?\n");  // flush the queue
	
	int r = 0;
	while (r != 272) { r = std::stoi(sendRec("STAT:OPER?\n")); };
	
	std::string vals = sendRec("FETC?\n");
	std::string segment;
	std::vector<double> vals_d (Fluke::N_ACTIVE_CH, 0.0);  // out vec with all zeros
	
	std::stringstream vals_stream (vals);
	for (int i = 0; i < Fluke::N_ACTIVE_CH; i++)  // parse the whole response
	{
		std::getline(vals_stream, segment, ',');
		vals_d[i] = std::stod(segment);
	}
	
	return vals_d;
}

Fluke::~Fluke()
{
	
}
