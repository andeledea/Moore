#pragma once
#include "Scale.h"
#include "DLLFunc.h"

#define ULONG unsigned long
#define USHORT unsigned short

#define ENC_INCREMENTAL 0
#define ENC_ENDAT 1
#define SIG_11UA 0
#define SIG_VPP 1


/**
* Interface for the IK220 optical scale reader
* PCI board
* */
class IkOptical : PosInstr
{
public:
	using PosInstr::PosInstr;

	/**
	* Conncts the IK220 and finds all the conncted
	* optical scales
	* */
	void connect();

	/**
	* Sets the ecoder type to ENC_INCREMENTAL
	* and the signal type to SIG_11UA
	* for each optical scale
	* */
	void setParams();

	/**
	* Reads all the conncted scales and displays the 
	* values on the terminal
	* */
	double readInstr();
	
	/**
	* Returns the yScale object of the machine
	* that can be integrated in an axis object as
	* an encoder
	* 
	* @return yScale: the y optical Scale object
	* */
	Scale getYscale() { return yScale; };

	/**
	* Returns the zScale object of the machine
	* that can be integrated in an axis object as
	* an encoder
	*
	* @return zScale: the z optical Scale object
	* */
	Scale getZscale() { return zScale; };

	~IkOptical();
	
	
private:
	ULONG	IKCard[16];								// Port address of IK card(s)
	USHORT	Ax;										// Axis number

	char	VersCard[20], VersDrv[20], VersDll[20];		// Version text of card, driver and DLL
	ULONG	DllStatus, DllInfo;
	
	Scale yScale, zScale;
};



