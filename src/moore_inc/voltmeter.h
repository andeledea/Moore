#pragma once

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include "ni4882.h"
#include "AmbInstr.h"

#define PRESSURE_A 6002.66
#define PRESSURE_B 79755.63

#define HUMIDITY_A 100.04
#define HUMIDITY_B -50.48

/**
* Class for ambiental parameters with old moore setup
* When the intruments are renewed split the classes in
* barometer and igrometer
* */
class Voltmeter : AmbInstr
{
public:
	using AmbInstr::AmbInstr;

	/**
	* Connects the voltmeter using the GPIB interface
	* */
	void connect();

	/**
	* Sets the parameters of the instrument
	* */
	// https://xdevs.com/doc/HP_Agilent_Keysight/3458A/EPSG084364.pdf
	void setParams();

	/**
	* Reads the ambient pressure value
	* */
	double readPressure();

	/**
	* Reads the ambient humidity value
	* */
	double readHumidity();

	~Voltmeter();

private:
	int handle = -1;

	/**
	* GPIB interface com handler 
	*/
	double ask(const std::string send, bool retFlag) 
	{
		ibwrt(this->handle, send.c_str(), send.length());

		if (retFlag)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			char ret[50];
			ibrd(this->handle, ret, 20); //leggo id
			ret[Ibcnt()] = '\0';
			std::cout << send << " : " << ret << std::endl;

			try
			{
				return std::stod(ret);
			}
			catch (std::invalid_argument e) // if its not a float skip the conversion
			{
				return -1;
			};
		}
		return -1;
	};
};