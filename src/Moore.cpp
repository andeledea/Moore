#include <iostream>
#include <iomanip>
#include <conio.h>
#include <direct.h>
#include <string>
#include <cmath>

#include "Moore.h"

void Moore::init()
{
	// CONFIG THE SERIAL COM WITH UC
	char port[] = "COM7";
	if (ser.OpenSerialPort(port, CBR_57600)) // apro la com seriale
	{
		std::cout << "[INFO] STM32 connesso" << std::endl;
	}
	else
	{
		std::cout << "[ERROR] STM32 non connesso" << std::endl;
		throw std::runtime_error("Unable to connect micro");
	}

	// CONNECT THE OPTICAL SCALES
	ik.connect();
	yScale = ik.getYscale();
	zScale = ik.getZscale();

	yScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Y);
	zScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Z);

	key.connect();

	// INIT THE 3 AXIS
	Xaxis.init((PosInstr*)&key, ser, x_lab);
	Yaxis.init((PosInstr*)&yScale, ser, y_lab);
	Zaxis.init((PosInstr*)&zScale, ser, z_lab);

	// INIT THE CARY
	cary.connect();
    // std::thread cary_safety_t([this](){
    //     while(true) {
    //         cary.readInstr();
    //         if (abs(cary.getValue()) > 0.05) this->emergencyStop();
    //     };
    // });
    // cary_safety_t.detach();

	Xaxis.setMeasInstrument((PosInstr*) &cary);

	// acc, start, max, stop 
	Xaxis.setRamp(70000, 4000, 65535, 20);
	Yaxis.setRamp(70000, 4000, 65535, 20, inv_mov);
	Zaxis.setRamp(70000, 40000, 65535, 20000);
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
	abs_val.x = this->Xaxis.getPosition();
	abs_val.y = this->Yaxis.getPosition();
	abs_val.z = this->Zaxis.getPosition();

	rel_val = abs_val - zeroPos;
}

pos Moore::getAbsPositionWithInstr()
{
    return (pos){
		.x = Xaxis.getPositionWithInstr(),
		.y = Yaxis.getPositionWithInstr(),
		.z = Zaxis.getPositionWithInstr()
	};
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

void Moore::keyboardMove(bool z_contr)
{
	std::thread zt;
	std::thread xt(&Asse::keyboardControl, &Xaxis, 'd', 'a');
	std::thread yt(&Asse::keyboardControl, &Yaxis, 'w', 's');
	if (z_contr) zt = std::thread(&Asse::keyboardControl, &Zaxis, 'f', 'r');

	xt.join();
	yt.join();
	if (z_contr) zt.join();
}

void Moore::emergencyStop()
{
	std::cout << "[ERROR] Emergency stop activated!!"  << std::endl;
	this->Xaxis.setLock(true);
	this->Yaxis.setLock(true);
	this->Zaxis.setLock(true);
}

Moore::~Moore()
{
	ser.CloseSerialPort();
}
