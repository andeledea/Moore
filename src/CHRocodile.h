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

class CHRocodile : PosInstr
{
public:
	using PosInstr::PosInstr;

	void connect();
	void setParams();

	float readInstr();
	// int cnt_check = check_period;

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