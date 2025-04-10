#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Scale.h"

void Scale::setParams(ULONG encoderType, ULONG signalType, double signalPeriod)
{
	if (!IK220WritePar(ax, 1, encoderType)) std::cout << "[ERROR] Error in writePar";
	else std::cout << "[INFO] Setting Encoder Type to incremental" << std::endl;
			
	if (!IK220WritePar(ax, 2, signalType)) std::cout << "[ERROR] Error in writePar";
	else std::cout << "[INFO] Setting Signal Type to 11uA" << std::endl;
	
	scaleParams.encoderType = encoderType;
	scaleParams.signalType = signalType;
	scaleParams.signalPeriod = signalPeriod;
}

HEIDparams Scale::getParams()
{
	if (!IK220ReadPar(ax, 1, &scaleParams.encoderType)) std::cout << "[ERROR] Error in ReadPar";			
	if (!IK220ReadPar(ax, 2, &scaleParams.signalType)) std::cout << "[ERROR] Error in ReadPar";
	//if (!IK220ReadPar(ax, 5, &scaleParams.signalPeriod)) std::cout << "[ERROR] Error in ReadPar";
	
	return scaleParams;
}

double Scale::readInstr()
{
	double	CntVal; // Counter value
	
	if (!IK220Read48 (ax, 0, &CntVal))  
		std::cout << "[ERROR] Error: IK220Read48 card " << ax;	// Read counter value 
	return CntVal * scaleParams.signalPeriod;
}

Scale::~Scale()
{
	
}