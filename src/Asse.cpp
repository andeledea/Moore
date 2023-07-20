#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "Asse.h"

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
	// position = instrPT->readInstr();
}

float Asse::getPosition()
{
	position = instrPT -> readInstr();
	return position;
}

void Asse::setPosition(float targetPosition)
{
	float startP = instrPT->readInstr(); //posizione di partenza

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

	float distance = abs(targetPosition - startP); //calcolo la dist totale
	float travel = 0;

	while (distance - travel > 0) // finchè non raggiungo il target
	{
		setVelocity(distance, travel); //imposto la velocità istantanea
		do
		{
			position = instrPT->readInstr();
		} while (position == 0.0f); // non accettare gli 0.0 perche sono errori di lettura
		travel = abs(position - startP); //calcolo il percorso fatto
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	};
	
	velocity = 0;
	sendVelocityToMicro();

	lock = true;
	sendCommandToMicro();
}

void Asse::setRamp(float acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement)
{
	acceleration = acc;
	startV = startv;
	maxV = maxv;
	stopV = stopv;

	invertedMovement = invertMovement;
}

Asse::~Asse()
{
	//ser.CloseSerialPort(); //lo distruggo nella seriale
}

void Asse::setVelocity(float dist, float trav)
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
	
	// std::cout << (int) toSend << " " << position << std::endl;
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
	float bound = (measPT -> max - measPT -> min) * 10 / 100;
	
	while (measuring)
	{
		current = measPT -> readInstr();
		disp = abs(current - pos);
		if (current > (pos + bound)) setPosition(getPosition() - disp);
		if (current < (pos - bound)) setPosition(getPosition() + disp);
	}
}

void Asse::startMeasure(int v, bool d)
{
	lock = false;
	measuring = true;
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
	measuring = false;
	
	velocity = 0;
	sendVelocityToMicro();
	lock = true;
	sendCommandToMicro();
}
