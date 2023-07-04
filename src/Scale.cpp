#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Scale.h"

void Scale::setParams(ULONG encoderType = ENC_INCREMENTAL, ULONG signalType = SIG_11UA)
{
	if (!IK220WritePar(ax, 1, encoderType)) std::cout << "Error in writePar";
	else std::cout << "Setting Encoder Type to incremental" << std::endl;
			
	if (!IK220WritePar(ax, 2, signalType)) std::cout << "Error in writePar";
	else std::cout << "Setting Signal Type to 11uA" << std::endl;
	
	scaleParams.encoderType = encoderType;
	scaleParams.signalType = signalType;
}

HEIDparams Scale::getParams()
{
	if (!IK220ReadPar(ax, 1, &scaleParams.encoderType)) std::cout << "Error in ReadPar";			
	if (!IK220ReadPar(ax, 2, &scaleParams.signalType)) std::cout << "Error in ReadPar";
	
	return scaleParams;
}

float Scale::readInstr()
{
	double	CntVal; // Counter value
	while (!_kbhit())
	{
		if (!IK220Read48 (ax, 0, &CntVal))  std::cout << "Error: IK220Read48 card " << ax;	// Read counter value 
		else printf ("Axis %d: %12.4f     ", ax, CntVal);
		
		printf ("\r");
		Sleep (200);
	}
	_getch();
	return (float) CntVal;
}

Scale::~Scale()
{
	
}