#include <stdio.h>
#include <conio.h>
#include <iostream>

#include "IkOptical.h"

void IkOptical::connect()
{
	if (!IK220Find (IKCard))		// Look for IK 220
	{
		std::cout << "Error: IK220Find" << std::endl;								
		if (!IK220DllStatus (&DllStatus, &DllInfo))  std::cout << "Error: IK220DllStatus" << std::endl;		// Read DLL-Status
		std::cout << "DLL-Status: " << DllStatus << " DLL-Info: " << DllInfo << std::endl;
		_getch();
	}
	
	for (Ax=0; Ax<2; Ax++)
	{
		if (IKCard[Ax])
		{
			if (!IK220Init (Ax))  std::cout << "Error: IK220Init axis " << Ax << std::endl;					// Initialize IK 220
			else 
			{
				std::cout << "Axis " << Ax << " initialized  -  ";

				if (!IK220Version (Ax, &VersCard[0], &VersDrv[0], &VersDll[0])) printf ("Error: IKVersion\n");				// Read port address of IK card(s)
				else 
					std::cout << "Card: " << VersCard << VersDrv << VersDll << std::endl;   
			}
		}
	}
	
	
	if (IKCard[0]) 
	{
		std::cout << "IK 220 " << 0 << " at address: 0x"  << IKCard[0] << std::endl;
		yScale.setIkCard(IKCard[0]);
		yScale.setAx(0);
	}
	if  (IKCard[1]) 
	{
		std::cout << "IK 220 " << 1 << " at address: 0x"  << IKCard[1] << std::endl;
		zScale.setIkCard(IKCard[1]);
		zScale.setAx(1);
	}
	std::cout << std::endl;
	return;
}

void IkOptical::setParams()
{	
	for (Ax=0; Ax<16; Ax++)
	{
		if (IKCard[Ax]) 
		{
			if (!IK220WritePar(Ax, 1, ENC_INCREMENTAL)) std::cout << "Error in writePar";
			else std::cout << "Setting Encoder Type to incremental" << std::endl;
			
			if (!IK220WritePar(Ax, 2, SIG_11UA)) std::cout << "Error in writePar";
			else std::cout << "Setting Signal Type to 11uA" << std::endl;
		}
	}
	std::cout << std::endl;
	return;
}

float IkOptical::readInstr() // read all the axis connected in a loop
{
	double	CntVal; // Counter value
	while (!_kbhit())
	{
		for (Ax=0; Ax<2; Ax++)
			if (IKCard[Ax])
			{
				if (!IK220Read48 (Ax, 0, &CntVal))  std::cout << "Error: IK220Read48 card " << Ax;	// Read counter value 
				else printf ("Axis %d: %12.4f     ", Ax, CntVal);
			}
		printf ("\r");
		Sleep (200);
	}
	_getch();
	return (float) CntVal;
}

IkOptical::~IkOptical()
{
	
}
