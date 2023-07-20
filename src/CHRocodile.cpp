#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <cmath>

#include "CHRocodile.h"
#include "CHRocodileDLLInterface.h"

void CHRocodile::connect()
{
	std::string strConnInfo = "IP: 192.168.170.2, Port: 7891";
	int nRes = 0;
	if (strConnInfo.find("COM") != std::string::npos)
		nRes = OpenConnection(strConnInfo.c_str(), 0, &handle);
	else
		nRes = OpenConnection(strConnInfo.c_str(), 1, &handle);
	if (nRes < 0) //connection error
	{
		throw std::runtime_error("Connection not estblished");
	}
}

void CHRocodile::setParams()
{
	int MeasMode = 0;
	int ScanRate = 2000;
	int DataAverage = 1;
	
	max = 0.10;  // mm
	min = 0;

	// Set measurement mode to confocal mode:
	SetMeasuringMode(handle, MeasMode);
	// Set sample rate to 4kHz:
	SetScanRate(handle, ScanRate);
	// Set Averaging to 1 (data written to file):
	SetDataAverage(handle, DataAverage);

	int an[] = { 83,256,257 }; // get sample counter, distance and intensity
	SetOutputSignals(handle, an, 3);

	SetOpticalProbe(handle, Probe122);
}

float CHRocodile::readInstr()
{
	std::string str;

	double* p = 0;
	
	// Read next sample (which consists of nCount signals as defined using
	// TCHRocodile::SetOutputSignals(...)).
	// Every sample comes as an array of double.	
	int nCount = 0;
	
	CHRparams to_ret;
	int j = 0;
	do
	{
		j++;
		int nRes = GetLastSample(handle, &p, &nCount);
		//if get new sample, output the sample
		if (nRes > 0)
		{
			// sample counter output by device // distance in micrometer // intensity in percent
			to_ret.nSample = int(*(p++));
			to_ret.dist = *(p++);
			to_ret.inty = *(p++);
			Sleep(5);
		}
	} while (std::isnan(to_ret.dist) && (j < 4));
	if (j == 4) throw std::runtime_error("Could not read CHR sample");
	else
		if(to_ret.dist > 97 || to_ret.dist < 3) throw std::runtime_error("CHR out of range");
		else {
			this->cnt_check = check_period;
			value = to_ret.dist / 1000; // convert in mm
			return to_ret.dist / 1000;
		}
}

CHRocodile::~CHRocodile()
{
	CloseConnection(handle);
}
