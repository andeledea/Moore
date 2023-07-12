#pragma once
#include <thread>
#include "Asse.h"

struct pos {
	float x;
	float y;
	float z;
};

class Moore
{
public:
	pos getCurrentPosition() { return currentPos; }
	void setCurrentPosition(float x, float y, float z);

	float getX() { return currentPos.x; }
	float getY() { return currentPos.y; }
	float getZ() { return currentPos.z; }

	void setAxes(Asse x, Asse y, Asse z);

	void startProcess();
	void stopProcess();

private:
	Asse Xaxis, Yaxis, Zaxis;

	std::thread t;

	pos currentPos;

	/*
	Process used to loop read the position
	of the Moore machine
	*/
	void process();
};
