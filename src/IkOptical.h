#pragma once
#include "Scale.h"
#include "DLLFunc.h"

#define ULONG unsigned long
#define USHORT unsigned short

#define ENC_INCREMENTAL 0
#define ENC_ENDAT 1
#define SIG_11UA 0
#define SIG_VPP 1

class IkOptical : PosInstr
{
public:
	using PosInstr::PosInstr;

	void connect();
	void setParams();

	float readInstr();
	
	Scale getYscale() {return yScale;};
	Scale getZscale() {return zScale;};

	~IkOptical();
	
	
private:
	ULONG	IKCard[16];								// Port address of IK card(s)
	USHORT	Ax;										// Axis number

	char	VersCard[20], VersDrv[20], VersDll[20];		// Version text of card, driver and DLL
	ULONG	DllStatus, DllInfo;
	
	Scale yScale, zScale;
};



