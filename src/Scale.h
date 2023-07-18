#pragma once

#include "DLLFunc.h"
#include "PosInstr.h"

#define ULONG unsigned long
#define USHORT unsigned short

#define ENC_INCREMENTAL 0
#define ENC_ENDAT 1
#define SIG_11UA 0
#define SIG_VPP 1

#define M_SIG_PERIOD 0.020

struct HEIDparams
{
	ULONG encoderType;
	ULONG signalType;
	float signalPeriod;
};

class Scale : PosInstr
{
public:
	using PosInstr::PosInstr;
	
	void setIkCard(ULONG card) {ikcard = card;};
	ULONG getIkcard() {return ikcard;};
	void setAx(USHORT Ax) {ax = Ax;};
	USHORT getAx() {return ax;};
	
	void setParams(ULONG encoderType = ENC_INCREMENTAL, 
				   ULONG signalType = SIG_11UA,
				   float signalPeriod = M_SIG_PERIOD);
	HEIDparams getParams();
	
	float readInstr();
	
	~Scale();

private:
	ULONG ikcard;  // address
	USHORT ax;
	HEIDparams scaleParams;
};