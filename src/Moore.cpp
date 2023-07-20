#include <iostream>
#include <iomanip>
#include <conio.h>
#include "Moore.h"
#include "Asse.h"

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
	
	yScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Z); // change M_SIG_PERIOD_Y
	zScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Z);
	
	las.connect();
	las.setParams();
	
	// INIT THE 3 AXIS
	
	Xaxis.init((PosInstr*) &las, ser, x_lab);
	Yaxis.init((PosInstr*) &yScale, ser, y_lab);
	Zaxis.init((PosInstr*) &zScale, ser, z_lab);
	
	Xaxis.setRamp(100, 25, 63, 15);
	//Yaxis.setRamp(100, 25, 63, 15);
	Yaxis.setRamp(10, 30, 50, 15, inv_mov); // temporary
	Zaxis.setRamp(10, 20, 50, 15, inv_mov);
}

pos Moore::getCurrentPosition()
{
	currentPos.x = Xaxis.getPosition();
	currentPos.y = Yaxis.getPosition();
	currentPos.z = Zaxis.getPosition();
	
	return currentPos;
}

void Moore::setCurrentPosition(float x, float y, float z)
{
	std::thread xt(&Asse::setPosition, Xaxis, x);
	std::thread yt(&Asse::setPosition, Yaxis, y);
	// std::thread zt(&Asse::setPosition, Zaxis, z);
	
	yt.join();
	xt.join();
	// zt.join();
}

void Moore::moveX()
{
	pos p = getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;

	float xpos;
	std::cout << "Where to go? x ";
	std::cin >> xpos;
	Xaxis.setPosition(xpos);
	
	p = getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
}

void Moore::moveY()
{
	pos p = getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;

	float ypos;
	std::cout << "Where to go? y ";
	std::cin >> ypos;
	Yaxis.setPosition(ypos);
	
	p = getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
}

void Moore::measScales()
{
	std::cout << std::fixed << std::setw(6) << std::setprecision(4) << std::setfill('0');
	while (!_kbhit())
	{
		std::cout << yScale.readInstr() << "\t" << zScale.readInstr();
		printf("\r");
	}
	_getch();
}

void Moore::measCHR()
{
	CHRocodile CHR;
	CHR.connect();
	CHR.setParams();
	
	Yaxis.setMeasInstrument((PosInstr*) &CHR);
	Xaxis.startMeasure(20, dir_fore);
	Yaxis.startTracking();
	
}