#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

#include "PosInstr.h"

#define Probe122 0 //short range probe
#define Probe969 1

#define check_period 20

struct CHRparams
{
	int nSample;
	double dist;
	double inty;
};

/**
* Interface for the CHR confocal chromatic
* sersor from precitec
* */
class CHRocodile : PosInstr
{
public:
	using PosInstr::PosInstr;

	/**
	* Connects the ethernet CHR interface
	* */
	void connect();

	/**
	* Connects the ethernet CHR interface
	* */
	void setParams();

	/**
	* Reads the CHR confocal distance value
	*
	* @return value: the position of the probe (mm)
	* */
	double readInstr();

	~CHRocodile();

private:
	CHRparams retValues;
};

// int, float, double to string:
template<typename T>
static inline std::string ToString(const T& _val)
{
	std::stringstream oStr;
	oStr << _val;
	if (oStr.fail())
		throw std::runtime_error("String conversion failed.");
	return oStr.str();
}