#include <iostream>
#include "Moore.h"
#include "Asse.h"

void Moore::setCurrentPosition(float x, float y, float z)
{
	std::thread xt(&Asse::setPosition, Xaxis, x);
	std::thread yt(&Asse::setPosition, Yaxis, y);
	std::thread zt(&Asse::setPosition, Zaxis, z);

	xt.join();
	yt.join();
	zt.join();
}

void Moore::init()
{
	// CONFIFG THE SERIAL COM WITH UC
	
	char port[] = "COM5";
	if (ser.OpenSerialPort(port, CBR_57600)) // apro la com seriale
	{
		std::cout << "Asse connesso" << std::endl;
	}
	else
	{
		throw std::runtime_error("Unable to connect micro");
	}
	
	// CONNECT THE OPTICAL SCALES
	
	ik.connect();
	yScale = ik.getYscale();
	zScale = ik.getZscale();
	
	yScale.setParams();
	zScale.setParams();
	
	// INIT THE 3 AXIS
	
	Xaxis.init((PosInstr*) &las, ser, 'X');
	Yaxis.init((PosInstr*) &yScale, ser, 'Y');
	Zaxis.init((PosInstr*) &zScale, ser, 'Z');
	
	Xaxis.setRamp(500, 25, 255, 15);
	Yaxis.setRamp(15, 25, 150, 15);
	Zaxis.setRamp(2.0f, 180, 200, 5, inv_mov);
}
