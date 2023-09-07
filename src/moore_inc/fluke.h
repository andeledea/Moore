#pragma once

#include "client.h"
#include "AmbInstr.h"

/**
* Interface for the temperature scanner
* */
class Fluke : AmbInstr
{
public:
	using AmbInstr::AmbInstr;

	/**
	* Connects the FLUKE temperature scanner
	* */
	void connect();

	/**
	* Sets the parameters of the instrument
	* */
	void setParams();

	/**
	* Reads the temperature value
	*
	* @return value: the temperature (°C)
	* */
	double readInstr();

	~Fluke();

private:
	tcp_client client;
};

//file:///G:/.shortcut-targets-by-id/1RVmiALwZlHBjg4XAo509zcXMgPXSQBEG/Stazioni%20di%20misura/Moore/nuovo%20progetto/Strumenti/Fluke%201586a/1586a___pgeng0000.pdf
