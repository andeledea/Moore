#include <iostream>
#include <iomanip>
#include <thread>
#include <limits>
#include <chrono>
#include "Asse.h"

bool Asse::measuring = false;

Asse::Asse()
{
	lock = true;
}

Asse::Asse(PosInstr *instrument, SimpleSerial &serial, char name)
{
	lock = true;
	
	ser = serial;
	instrPT = instrument;
	axisName = name;
	
	position = 0;
}

void Asse::init(PosInstr* instrument, SimpleSerial& serial, char name)
{
	ser = serial;
	instrPT = instrument; 
	axisName = name;

	position = 0;
}

double Asse::getPosition()
{
	position = instrPT->readInstr();
	return position;
}

void Asse::setPos(double targetPosition, const char instr)
{
	double P;
	double startP = position; //posizione di partenza
	if (instr == 'm') startP = measPT->readInstr();

	if (invertedMovement == false)
	{
		if (startP < targetPosition) //decido la direzione del movimento
			direction = dir_fore;
		else
			direction = dir_back;
	}
	else
	{
		if (startP < targetPosition) //decido la direzione del movimento
			direction = dir_back;
		else
			direction = dir_fore;
	}
	
	lock = false;
	sendCommandToMicro();

	double distance = abs(targetPosition - startP); //calcolo la dist totale
	double travel = 0;

	while (distance - travel > 0) // finchè non raggiungo il target
	{
		setVelocity(distance, travel); //imposto la velocità istantanea
		if (instr == 'm') P = measPT->readInstr();
		P = position;
		travel = abs(P - startP); //calcolo il percorso fatto
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	};
	
	velocity = 0;
	sendVelocityToMicro();

	lock = true;
	sendCommandToMicro();
}

void Asse::setRamp(unsigned int acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement)
{
	acceleration = acc;
	startV = startv;
	maxV = maxv;
	stopV = stopv;

	invertedMovement = invertMovement;
}

void Asse::setVelocity(double dist, double trav)
{
	unsigned int old = velocity;
	if (trav < dist / 2) //accelerazione
	{
		velocity = (unsigned int) acceleration * trav;

		if (velocity <= startV)
			velocity = startV;
	}
	else //decelerazione
	{
		velocity = (unsigned int)acceleration * (dist - trav);

		if (velocity <= stopV)
			velocity = stopV;
	}

	if (velocity >= maxV) 
		velocity = maxV;
	
	if (velocity != old) sendVelocityToMicro();
}

void Asse::sendVelocityToMicro()
{
	char toSend = (char) velocity;
	
	toSend = toSend & 0b00111111 | axisName; // attach axis bits
	
	// std::cout << velocity << " " << (unsigned int) toSend << " " << position << std::endl;
	ser.WriteSerialPort(&toSend, 1);
}

void Asse::sendCommandToMicro()
{
	char toSend = 0b00000001;
	
	toSend |= axisName; // attach axis bits
	
	if (lock) toSend |= 0b00000010;  // attach lock bit
	if (!direction) toSend |= 0b00000100;  // attach dir bit
	
	// std::cout << (int) toSend << std::endl;
	ser.WriteSerialPort(&toSend, 1);
}

void Asse::retension(float pos, int v)
{
	this->startMeasure(v, dir_up);
	while(instrPT -> readInstr() < pos) {}
	this->stopMeasure();
}

void Asse::track(float pos)
{
	float current, disp;
	float k = maxV / (measPT -> max - measPT -> min); // constant for speed
	//constant calculated to have max speed if disp == range
	
	lock = false;
	bool prevDir = dir_up;
	
	direction = dir_up;
	sendCommandToMicro();
	
	while (Asse::measuring)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		current = measPT -> readInstr();
		disp = current - pos;  // distance from target
		
		// calculate direction
		if (disp > 0) direction = dir_down;
		else direction = dir_up;
		
		if (direction != prevDir) // if change direction 
		{
			//std::cout << "change!!" << std::endl;
			velocity = 0;
			sendVelocityToMicro();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			sendCommandToMicro();
		}
		velocity = (unsigned int) (abs(disp) * k);
		//std::cout << velocity << std::endl;
		if (velocity != 0) sendVelocityToMicro();
		
		prevDir = direction;
	}
	velocity = 0;
	sendVelocityToMicro();
	lock = true;
	sendCommandToMicro();
}

void Asse::findMeasCenter()
{
	float c = (measPT -> max + measPT -> min) / 2;
	
	startMeasure(startV, dir_down);
	float p;
	do {
		try
		{
			p = measPT -> readInstr();
		}
		catch (...)
		{
			p = std::numeric_limits<float>::infinity();
		}
	} while(p > c);
	stopMeasure();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	setInstrPosition(c);
}

void Asse::startMeasure(int v, bool d)
{
	lock = false;
	Asse::measuring = true;
	if (d) //decido la direzione del movimento
		direction = dir_fore;
	else
		direction = dir_back;
	
	sendCommandToMicro();

	velocity = v;
	sendVelocityToMicro();
}

void Asse::stopMeasure()
{
	Asse::measuring = false;
	
	velocity = 0;
	sendVelocityToMicro();
	lock = true;
	sendCommandToMicro();
}

Asse::~Asse()
{
	//ser.CloseSerialPort(); //lo distruggo nella seriale
}