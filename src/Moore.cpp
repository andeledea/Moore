#include <iostream>
#include <iomanip>
#include <conio.h>
#include <direct.h>
#include <string>
#include "Moore.h"

void Moore::init()
{
	// CONFIG THE SERIAL COM WITH UC
	char port[] = "COM5";
	if (ser.OpenSerialPort(port, CBR_57600)) // apro la com seriale
	{
		std::cout << "Asse connesso" << std::endl;
	}
	else
	{
		std::cout << "Asse NON connesso" << std::endl;
		throw std::runtime_error("Unable to connect micro");
	}

	// CONNECT THE OPTICAL SCALES
	ik.connect();
	yScale = ik.getYscale();
	zScale = ik.getZscale();

	yScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Y);
	zScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Z);

	las.connect();
	las.setParams();

	// INIT THE 3 AXIS
	Xaxis.init((PosInstr*)&las, ser, x_lab);
	Yaxis.init((PosInstr*)&yScale, ser, y_lab);
	Zaxis.init((PosInstr*)&zScale, ser, z_lab);

	// INIT THE CHR
	CHR.connect();
	CHR.setParams();

	Zaxis.setMeasInstrument((PosInstr*)&CHR);

	// INIT THE CARY
	cary.connect();
	
	cary.readInstr();

	Xaxis.setMeasInstrument((PosInstr*)&cary);

	// acc, start, max, stop 
	Xaxis.setRamp(100, 20, 63, 15);
	Yaxis.setRamp(100, 25, 63, 15);
	Zaxis.setRamp(100, 30, 63, 10, inv_mov);
}

void Moore::setAbsPosition(pos target)
{
	std::thread xt(&Asse::setPosition, &Xaxis, target.x);
	std::thread yt(&Asse::setPosition, &Yaxis, target.y);
	std::thread zt(&Asse::setPosition, &Zaxis, target.z);

	xt.join();
	yt.join();
	zt.join();
}

void Moore::updatePosition()
{
	abs.x = this->Xaxis.getPosition();
	abs.y = this->Yaxis.getPosition();
	abs.z = this->Zaxis.getPosition();

	rel = abs - zeroPos;
}

void Moore::defineZero()
{
	zeroPos = getAbsPosition();
}

void Moore::setRelPosition(pos target)
{
	target += zeroPos;
	setAbsPosition(target);
}

void Moore::moveInstr(double target)
{
	// TODO
}

Moore::~Moore()
{
	ser.CloseSerialPort();
}
