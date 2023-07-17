#include <iostream>
#include "Moore.h"
#include "Asse.h"

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
}

void Moore::moveY()
{
	pos p = getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;

	float ypos;
	std::cout << "Where to go? y ";
	std::cin >> ypos;
	Yaxis.setPosition(ypos);
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
	las.setParams();
	
	// INIT THE 3 AXIS
	
	Xaxis.init((PosInstr*) &las, ser, x_lab);
	Yaxis.init((PosInstr*) &yScale, ser, y_lab);
	Zaxis.init((PosInstr*) &zScale, ser, z_lab);
	
	Xaxis.setRamp(80, 20, 63, 15);
	Yaxis.setRamp(8, 30, 55, 25);
	Zaxis.setRamp(2.0f, 180, 200, 5, inv_mov);
}
