#pragma once
#include "SimpleSerial.h"
#include "Laser.h"

#define dir_back false
#define dir_fore true

#define dir_up false
#define dir_down true

#define inv_mov true

class Asse
{
public:
	Asse();
	Asse(PosInstr *instrument, SimpleSerial &serial, char name);

	void init(PosInstr* instrument, SimpleSerial& serial, char name);

	float getPosition(); //ritorna la pos attuale
	void setPosition(float targetPos); //per movimento no misura
	void setRamp(float acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement = false);

	bool isLocked();
	void lock();
	void unlock();
	void setDirectionForeward();
	void setDirectionBackward();

	void retension(float pos, int v);

	//inizia la misura a velocità v e direzione d (for = true)
	void startMeasure(int v = 1, bool d = true); 
	void stopMeasure(); //ferma l'asse per la misura

	inline void operator= (float& B) { this -> setPosition(B); }
	
	~Asse();

private:
	void setVelocity(float dist, float trav);
	void sendToMicro(std::string toSend);

	char axisName = 0;

	bool lockB = true;
	bool invertedMovement = false;

	int acceleration = 800.0f;
	unsigned int startV = 1;
	unsigned int maxV = 255;
	unsigned int stopV = 1;

	unsigned int velocity = 0;
	float position;

	SimpleSerial ser;
	PosInstr *instrPT;
};

