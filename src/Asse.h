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
	void setInstrPosition(float targetPos) { setPos(targetPos, measPT); }

	float getPosition(); //ritorna la pos attuale
	void setPosition(float targetPos) { setPos(targetPos, instrPT); }
	void setRamp(float acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement = false);

	bool isLocked() { return lock; };
	void retension(float pos, int v);

	//move the axis at the set speed
	void startMeasure(int v = 1, bool d = dir_fore); 
	void stopMeasure(); //ferma l'asse per la misura
	static bool measuring;

	inline void operator= (float& B) { this -> setPosition(B); }
	
	~Asse();

private:
	void setPos(float targetPos, PosInstr * instr);
	void setVelocity(float dist, float trav);
	void sendVelocityToMicro();
	void sendCommandToMicro();

	char axisName = x_lab;
	
	bool lock = true;
	bool direction = dir_fore;
	bool invertedMovement = false;

	int acceleration;
	unsigned int startV;
	unsigned int maxV;
	unsigned int stopV;

	unsigned int velocity = 0;
	float position;

	SimpleSerial ser;
	PosInstr *instrPT;
	PosInstr *measPT = nullptr;
};

