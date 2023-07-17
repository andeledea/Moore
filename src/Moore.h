#pragma once
#include <thread>
#include "Asse.h"
#include "IkOptical.h"
#include "Scale.h"
#include "SimpleSerial.h"

struct pos {
	float x;
	float y;
	float z;
};

class Moore
{
public:
	pos getCurrentPosition();
	void setCurrentPosition(float x, float y, float z);
	
	void moveX();
	void moveY();

	float getX() { return currentPos.x; }
	float getY() { return currentPos.y; }
	float getZ() { return currentPos.z; }

	void init();

private:
	SimpleSerial ser;

	Asse Xaxis, Yaxis, Zaxis;
	
	Laser las;
	IkOptical ik;
	Scale yScale, zScale;
	
	pos currentPos;
};
