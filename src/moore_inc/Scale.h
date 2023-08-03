#pragma once

#include "DLLFunc.h"
#include "PosInstr.h"

#define ULONG unsigned long
#define USHORT unsigned short

#define ENC_INCREMENTAL 0
#define ENC_ENDAT 1
#define SIG_11UA 0
#define SIG_VPP 1

#define M_SIG_PERIOD_Y 0.020
#define M_SIG_PERIOD_Z 0.004

struct HEIDparams
{
	ULONG encoderType;
	ULONG signalType;
	double signalPeriod;
};

/**
* Interface for a IK220 single optical scale
* */
class Scale : PosInstr
{
public:
	using PosInstr::PosInstr;
	
	void setIkCard(ULONG card) { ikcard = card; };
	ULONG getIkcard() { return ikcard; };
	void setAx(USHORT Ax) { ax = Ax; };
	USHORT getAx() { return ax; };
	
	/**
	* Sets the ecoder type and the signal type of
	* the optical scale
	* 
	* @param encoderType: ENC_INCREMENTAL (def val), ENC_ENDAT
	* @param signalType: SIG_11UA (def val), SIG_VPP
	* @param signalPeriod: the scale factor of the reading
	*	M_SIG_PERIOD_Y (def val), M_SIG_PERIOD_Z
	* */
	void setParams(ULONG encoderType = ENC_INCREMENTAL, 
				   ULONG signalType = SIG_11UA,
				   double signalPeriod = M_SIG_PERIOD_Y);

	/**
	* Returns the scale set parameters
	* 
	* @return scaleParams
	* */
	HEIDparams getParams();
	
	/**
	* Reads the encoder value
	* 
	* @return value: the current scale position
	* */
	double readInstr();
	
	~Scale();

private:
	ULONG ikcard;  // address
	USHORT ax;
	HEIDparams scaleParams;
};