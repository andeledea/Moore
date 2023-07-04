<<<<<<< HEAD
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <direct.h>
#include "Moore.h"

void Moore::init()
{
#ifndef GUI
	// CONFIFG THE SERIAL COM WITH UC
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
	
	Xaxis.init((PosInstr*) &las, ser, x_lab);
	Yaxis.init((PosInstr*) &yScale, ser, y_lab);
	Zaxis.init((PosInstr*) &zScale, ser, z_lab);

	// INIT THE CHR
	CHR.connect();
	CHR.setParams();

	Zaxis.setMeasInstrument((PosInstr*)&CHR);
#endif
	
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
	
=======
#include "Moore.h"
#include "Asse.h"

void Moore::setCurrentPosition(float x, float y, float z)
{
	xt = std::thread(Xaxis.setPosition, x);
	yt = std::thread(Yaxis.setPosition, y);
	zt = std::thread(Zaxis.setPosition, z);

>>>>>>> ab36b7f (Added moore class for movement)
	xt.join();
	yt.join();
	zt.join();
}

<<<<<<< HEAD
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

void Moore::measCHR(std::string nome_file, std::string path, int speed, bool track, long start, long stop)
{
	std::ofstream ost;

	_mkdir((path + nome_file).c_str());
	std::cout << "Saving at: " << path + nome_file << std::endl;

	ost.open(path + nome_file + "/data.CHRdat", std::ofstream::out);
	
	pos startP, stopP;
	startP.x = start;
	stopP.x = stop;
	
	setRelPosition(startP);  // beginning of measure
	
	Zaxis.findMeasCenter();

	pos p;
	bool dir = (start < stop);

	Xaxis.startMeasure(speed, dir);

	std::thread t;
	if (track) t = std::thread(&Asse::track, &Zaxis, 0.050f);

	auto finish = [](bool d, double sp, double pt)
	{
		if (d == dir_back) {
			if (pt < sp) return true;
		}
		else {
			if (pt > sp) return true;
		}
		return false;
	};
	
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		p = getRelPosition();
		ost << p.x << "," << p.y << "," << p.z << "," << CHR.readInstr() << std::endl;
		
		if (GetKeyState('S') & 0x8000 || finish(dir, stop, p.x)) // mi fermo se 's'
		{
			break;
		}
	}
	
	Xaxis.stopMeasure(); // mi fermo
	if (track) t.join();
}
=======
void Moore::setAxes(Asse x, Asse y, Asse z)
{
	Xaxis = x;
	Yaxis = y;
	Zaxis = z;
}

void Moore::startProcess()
{
	 t = std::thread(&Moore::process, this);
}

void Moore::stopProcess()
{
	t.detach();
}

void Moore::process()
{
	while (true)
	{
		currentPos.x = Xaxis.getPosition();
		currentPos.y = Yaxis.getPosition();
		currentPos.z = Zaxis.getPosition();
	}
}
>>>>>>> ab36b7f (Added moore class for movement)
