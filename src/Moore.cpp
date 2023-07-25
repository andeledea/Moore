#include <iostream>
#include <iomanip>
#include <conio.h>
#include <direct.h>
#include "Moore.h"
#include "Asse.h"

std::ostream& operator<<(std::ostream& os, const pos& p)
{
	os << p.x << " " << p.y << " " << p.z << std::endl;
	return os;
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

pos Moore::getAbsPosition()
{
	currentPos.x = Xaxis.getPosition();
	currentPos.y = Yaxis.getPosition();
	currentPos.z = Zaxis.getPosition();
	
	return currentPos;
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

void Moore::defineZero()
{
	zeroPos = getAbsPosition();
}

pos Moore::getRelPosition()
{
	pos current = getAbsPosition();
	current -= zeroPos;
	return  current;
}

void Moore::setRelPosition(pos target)
{
	target += zeroPos;
	setAbsPosition(target);
}

void Moore::moveX()
{
	pos p = getAbsPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;

	float xpos;
	std::cout << "Where to go? x ";
	std::cin >> xpos;
	Xaxis.setPosition(xpos);
	
	p = getAbsPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
}

void Moore::moveY()
{
	pos p = getAbsPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;

	float ypos;
	std::cout << "Where to go? y ";
	std::cin >> ypos;
	Yaxis.setPosition(ypos);
	
	p = getAbsPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
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
	std::string nome_file;
	std::cout << "Insert measure name: ";
	std::cin >> nome_file;

	std::ofstream ost;

	_mkdir(("C:/CHRmeasures/" + nome_file).c_str());

	ost.open("C:/CHRmeasures/" + nome_file + "/data.CHRdat", std::ofstream::out);
	
	CHRocodile CHR;
	CHR.connect();
	CHR.setParams();
	
	Yaxis.setMeasInstrument((PosInstr *) &CHR);
	Yaxis.findMeasCenter();
	
	char c;
	std::cout << "Dir f/b? ";
	std::cin >> c; 
	bool dir = (c == 'f')? dir_fore : dir_back;
	pos p;

	Xaxis.startMeasure(10, dir);
	std::thread t(&Asse::track, Yaxis, 0.050f);
	
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		p = getAbsPosition();
		ost << p.x << "," << p.y << "," << p.z << "," << CHR.readInstr() << std::endl;
		
		if (GetKeyState('S') & 0x8000) // mi fermo se 's'
		{
			break;
		}
	}
	
	Xaxis.stopMeasure(); // mi fermo
	t.join();
}