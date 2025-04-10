#include <stdio.h>
#include <conio.h>
#include <iostream>

#include "IkOptical.h"

void IkOptical::connect()
{
	if (!IK220Find (IKCard))		// Look for IK 220
	{
		std::cout << "[ERROR] IK220 not found" << std::endl;								
		if (!IK220DllStatus (&DllStatus, &DllInfo))  std::cout << "[ERROR] IK220DllStatus" << std::endl;		// Read DLL-Status
		std::cout << "[INFO] DLL-Status: " << DllStatus << " DLL-Info: " << DllInfo << std::endl;
		_getch();
	}
	
	for (Ax=0; Ax<2; Ax++)
	{
		if (IKCard[Ax])
		{
			if (!IK220Init (Ax))  std::cout << "[ERROR] IK220Init axis " << Ax << std::endl;					// Initialize IK 220
			else 
			{
				if (!IK220Version (Ax, &VersCard[0], &VersDrv[0], &VersDll[0])) std::cout << "[ERROR] IKVersion\n";				// Read port address of IK card(s)
				else 
					std::cout << "[INFO] Card " << Ax << "initialized" << VersCard << VersDrv << VersDll << std::endl;   
			}
		}
	}
	
	
	if (IKCard[0]) 
	{
		std::cout << "[INFO] IK 220 " << 0 << " at address: 0x"  << IKCard[0] << std::endl;
		yScale.setIkCard(IKCard[0]);
		yScale.setAx(0);
	}
	if  (IKCard[1]) 
	{
		std::cout << "[INFO] IK 220 " << 1 << " at address: 0x"  << IKCard[1] << std::endl;
		zScale.setIkCard(IKCard[1]);
		zScale.setAx(1);
	}
	return;
}

void IkOptical::setParams()
{	
	for (Ax=0; Ax<16; Ax++)
	{
		if (IKCard[Ax]) 
		{
			if (!IK220WritePar(Ax, 1, ENC_INCREMENTAL)) std::cout << "[ERROR] Error in writePar";
			else std::cout << "[INFO] Setting Encoder Type to incremental" << std::endl;
			
			if (!IK220WritePar(Ax, 2, SIG_11UA)) std::cout << "[ERROR] Error in writePar";
			else std::cout << "[INFO] Setting Signal Type to 11uA" << std::endl;
		}
	}
	std::cout << std::endl;
	return;
}

double IkOptical::readInstr() // read all the axis connected in a loop
{
	double	CntVal; // Counter value
	while (!_kbhit())
	{
		for (Ax=0; Ax<2; Ax++)
			if (IKCard[Ax])
			{
				if (!IK220Read48 (Ax, 0, &CntVal))  std::cout << "[ERROR] Error: IK220Read48 card " << Ax;	// Read counter value 
				else printf ("[INFO] Axis %d: %12.4f     ", Ax, CntVal);
			}
		printf ("\r");
		Sleep (200);
	}
	_getch();
	return CntVal;
}

IkOptical::~IkOptical()
{
	
}
