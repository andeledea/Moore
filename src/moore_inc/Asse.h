#pragma once
#include <thread>
#include "SimpleSerial.h"
#include "Laser.h"

#define dir_back false
#define dir_fore true

#define dir_up false
#define dir_down true

#define inv_mov true

#define x_lab 0b00000000
#define y_lab 0b01000000
#define z_lab 0b10000000

class Asse
{
public:
	Asse();
	Asse(PosInstr *instrument, SimpleSerial &serial, char name);

	void init(PosInstr* instrument, SimpleSerial& serial, char name);
	
	void setMeasInstrument(PosInstr* instrument) { measPT = instrument; }
	void track(float pos);
	void findMeasCenter();
	void setInstrPosition(float targetPos) { setPos(targetPos, 'm'); }

	double getPosition(); //ritorna la pos attuale
	void setPosition(double targetPos) { setPos(targetPos, 'i'); }
	void setRamp(double acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement = false);

	bool isLocked() { return lock; };
	void retension(float pos, int v);

	void startMeasure(int v = 1, bool d = dir_fore);
	void stopMeasure();
	static bool measuring;

	inline void operator= (double& B) { this -> setPosition(B); }
	
	~Asse();

private:
	void setPos(double targetPos, const char instr); // if instr == 'm' regulate measuring instr
	void setVelocity(double dist, double trav);
	void sendVelocityToMicro();
	void sendCommandToMicro();

	char axisName = x_lab;
	
	bool lock = true;
	bool direction = dir_fore;
	bool invertedMovement = false;

	double acceleration;
	unsigned int startV;
	unsigned int maxV;
	unsigned int stopV;

	unsigned int velocity = 0;
	double position;

	SimpleSerial ser;
	PosInstr *instrPT;
	PosInstr *measPT = nullptr;
};

