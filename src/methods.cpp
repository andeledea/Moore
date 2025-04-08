#include <iostream>
#include <iomanip>
#include <direct.h>
#include <conio.h>
#include <cmath>
#include "methods.h"

#define CHRrange 0.1f //in millimetri

void move(Asse& asse)
{
	std::cout << std::fixed << std::setw(11) << std::setprecision(6) << std::setfill('0');
	std::cout << "You are at: " << asse.getPosition() << std::endl;

	float pos;
	std::cout << "Where to go? ";
	std::cin >> pos;
	asse.setPosition(pos);
	std::cout << "You are at: " << asse.getPosition() << std::endl;
}

void measure(Asse& asseX, Asse&asseY, Laser& las, CHRocodile& CHR)
{
	std::string nome_file;
	std::cout << "Insert measure name: ";
	std::cin >> nome_file;

	std::ofstream ost, stitching_ost;

	_mkdir(("C:/CHRmeasures/" + nome_file).c_str());

	ost.open("C:/CHRmeasures/" + nome_file + "/data.CHRdat", std::ofstream::out);
	stitching_ost.open("C:/CHRmeasures/" + nome_file + "/stitching.CHRdat", std::ofstream::out);

	asseY.setPosition(50);	// mi metto lontano
	Sleep(2000);
	asseY.retension(50, 100);
	Sleep(8000);

	bool end = false;
	bool dir = dir_fore;

	float totalDisplacement = 0;
	float currentDisplacement = 0;

	float startP = asseX.getPosition();
	float sampleLength = 45; // mm

	std::cout << "Starting from: " << startP << std::endl;

	while (!end)
	{
		float pos = 0;
		float dis = 75;

		asseX.startMove(50, dir); // inzio a misurare
		while (dis < 85 && dis > 15)
		{
			pos = las.readInstr();
			dis = CHR.readInstr();
			std::cout << pos << '\t' << dis << std::endl;
			ost << pos << '\t' << dis + totalDisplacement << std::endl;
			Sleep(100); // un punto ogni 100 ms

			if (GetKeyState('S') & 0x8000 || measFinish(dir, sampleLength, startP, pos)) // mi fermo se 's' o fine del campione
			{
				end = true;
				break;
			}
		} // finchè sono in range misuro
		asseX.stopMove(); // mi fermo
		if (dis > 85)
		{
			asseY.setPosition(25); // mi muovo in basso
			Sleep(2000);
			asseY.retension(25, 100);
		}
		else if (dis < 15)
		{
			asseY.setPosition(75);
		}
		if (!end) {
			Sleep(10000); // aspetto assestamento y
			stitching_ost << pos << '\t' << dis + totalDisplacement << std::endl;
			totalDisplacement += dis - CHR.readInstr();
		}
	}
	ost.close();
	stitching_ost.close();
}

void calibrate(Asse& asse, CHRocodile& CHR)
{
	std::ofstream ost;

	ost.open("C:/CHRmeasures/calib1.CHRdat", std::ofstream::out);
	CHR.setParams();

	float pos = 0;
	float dis = 0;
		
	findCHRz(asse, CHR, dir_back);

	int nPoints = 20;

	float initPos = asse.getPosition();
	float travel = 0;

	for (int i = 0; i < nPoints; i++)
	{
		asse.startMove(10, dir_fore);
		while (travel < (i + 1) * CHRrange / nPoints)
		{
			travel = abs(asse.getPosition() - initPos);
		}
		asse.stopMove();
		Sleep(2000);

		pos = asse.getPosition();
		dis = CHR.readInstr();
		ost << travel << '\t' << dis << std::endl;
	};

	ost.close();
}

void findCHRz(Asse& asse, CHRocodile& CHR, bool lr)
{
	float dis = 0;

	asse.startMove(10, lr);
	do
	{
		dis = CHR.readInstr();
	} while (dis > 0 || std::isnan(dis));
	asse.stopMove();

	//ho raggiunto lo 0 del CHR
	std::cout << "You are at CHR = " << CHR.readInstr() << ", program will continue in 2s" << std::endl;
	Sleep(2000);
}

bool measFinish(bool dir, float sampleL, float startP, float pos) 
{
	float endP;
	if (dir == dir_back) { // parto da 45 vado a 0
		endP = startP - sampleL;
		if (pos < endP) return true;
	}
	else { // parto da 0 e vado a 45
		endP = startP + sampleL;
		if (pos > endP) return true;
	}
	return false;
}

void measScale(IkOptical& scale)
{
	scale.connect();
	Scale Y = scale.getYscale();
	Scale Z = scale.getZscale();
	
	Y.setParams();
	Z.setParams();
	
	float v;
	while (!_kbhit())
	{
		v = Y.readInstr();
		std::cout << v;
		printf("\r");
	}
	_getch();
}