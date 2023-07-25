#pragma once
#include <thread>
#include "Asse.h"
#include "CHRocodile.h"
#include "IkOptical.h"
#include "Scale.h"
#include "SimpleSerial.h"

struct pos {
	float x = 0;
	float y = 0;
	float z = 0;
	
	pos& operator-=(const pos& lr) 
	{
		this->x -= lr.x;		
		this->y -= lr.y;		
		this->z -= lr.z;

		return *this;
	}
	pos& operator+=(const pos& lr) 
	{
		this->x += lr.x;		
		this->y += lr.y;		
		this->z += lr.z;		
		
		return *this;
	}
	friend std::ostream& operator<<(std::ostream&, const pos&);
};

class Moore
{
public:
	pos getAbsPosition();
	void setAbsPosition(pos target);
	void defineZero();
	pos getRelPosition();
	void setRelPosition(pos target);
	
	
	void moveX();
	void moveY();
	
	void moveInstr();

	float getX() { return currentPos.x; }
	float getY() { return currentPos.y; }
	float getZ() { return currentPos.z; }
	
	void measScales();
	void measCHR();

	void init();

private:
	SimpleSerial ser;

	Asse Xaxis, Yaxis, Zaxis;
	
	Laser las;
	IkOptical ik;
	Scale yScale, zScale;
	
	pos currentPos;
	pos zeroPos;
};
