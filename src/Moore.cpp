#include <iostream>
#include <iomanip>
#include <conio.h>
#include <direct.h>
#include "Moore.h"

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

void Moore::setAbsPosition(pos target)
{
	std::thread xt(&Asse::setPosition, Xaxis, target.x);
	std::thread yt(&Asse::setPosition, Yaxis, target.y);
	//std::thread zt(&Asse::setPosition, Zaxis, target.z);
	
	yt.join();
	xt.join();
	//zt.join();
}

void Moore::updatePosition()
{
	while (true)
	{
		abs.x = Xaxis.getPosition();
		abs.y = Yaxis.getPosition();
		abs.z = Zaxis.getPosition();

		rel = abs - zeroPos;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
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

void Moore::moveInstr()  // metodo da eliminare
{
	CHRocodile CHR;
	CHR.connect();
	CHR.setParams();
	
	Yaxis.setMeasInstrument((PosInstr *) &CHR);
	
	float ypos;
	std::cout << "Where to go? y ";
	std::cin >> ypos;
	Yaxis.setInstrPosition(ypos);
}

void Moore::measCHR()
{
	std::string nome_file;
	std::cout << "Insert measure name: ";
	std::cin >> nome_file;

	std::ofstream ost;

	_mkdir(("C:/CHRmeasures/" + nome_file).c_str());

	ost.open("C:/CHRmeasures/" + nome_file + "/data.CHRdat", std::ofstream::out);
	
	CHRocodile CHR;
	CHR.connect();
	CHR.setParams();
	
	pos startP, stopP;
	startP.x = 6.0;
	stopP.x = -6.0;
	
	setRelPosition(startP);  // beginning of measure
	
	Yaxis.setMeasInstrument((PosInstr *) &CHR);
	Yaxis.findMeasCenter();
	
	// c;
	// std::cout << "Dir f/b? ";
	// std::cin >> c; 
	// bool dir = (c == 'f')? dir_fore : dir_back;
	bool dir = dir_back;
	pos p;

	Xaxis.startMeasure(10, dir);
	std::thread t(&Asse::track, Yaxis, 0.050f);
	
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		p = getRelPosition();
		ost << p.x << "," << p.y << "," << p.z << "," << CHR.readInstr() << std::endl;
		
		if (GetKeyState('S') & 0x8000 || p.x < stopP.x) // mi fermo se 's'
		{
			break;
		}
	}
	
	Xaxis.stopMeasure(); // mi fermo
	t.join();
}