#include <iostream>
#include <iomanip>
#include <thread>
#include <limits>
#include <chrono>
#include <cmath>
#include "Asse.h"

bool Asse::measuring = false;

Asse::Asse()
{
	lock = true;
}

Asse::Asse(PosInstr* instrument, SimpleSerial& serial, char name)
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
	double tempP;
	do
	{ 
		tempP = instrPT->readInstr();
		// std::cout << "[INFO] pos: " << position << '\r';
		// if (std::isnan(position)) std::cerr << "[ERROR] pos is NAN" << std::endl;
	} while (std::isnan(tempP));
	position = tempP;
	return position;
}

void Asse::setPos(double targetPosition, const char instr)
{
	double P;
	double startP = position; //posizione di partenza
	if (instr == 'm') startP = measPT->readInstr();

	if (startP < targetPosition) //decido la direzione del movimento
		direction = dir_fore;
	else
		direction = dir_back;

	if (invertedMovement) direction = !direction;	
	
	lock = false;

	double distance = abs(targetPosition - startP); //calcolo la dist totale
	double travel = 0;

	while (distance - travel > 0) // finchè non raggiungo il target
	{
		setVelocity(distance, travel); //imposto la velocità istantanea
		P = position;
		if (instr == 'm') P = measPT->readInstr();
		travel = abs(P - startP); //calcolo il percorso fatto
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	};
	
	velocity = 0;
	lock = true;
	sendVelocityToMicro();
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
	// f"{activeaxis}{'e' if enable else 'd'}{'b' if direction else 'f'}{speed}\r"
	std::string toSend = "";
	toSend += axisName;
	toSend += (lock) ? 'd' : 'e';
	toSend += (direction) ? 'f' : 'b';
	toSend += std::to_string(velocity);
	toSend += '\r';

	ser.WriteSerialPort(toSend.c_str());
}

// void Asse::retension(float pos, int v)
// {
// 	this->startMove(v, dir_up);
// 	while(instrPT -> readInstr() < pos) {}
// 	this->stopMove();
// }

// void Asse::track(float pos)
// {
// 	float current, disp;
// 	float k = maxV / (measPT -> max - measPT -> min); // constant for speed
// 	//constant calculated to have max speed if disp == range
	
// 	lock = false;
// 	bool prevDir = dir_up;
	
// 	direction = dir_up;
// 	sendCommandToMicro();
	
// 	while (Asse::measuring)
// 	{
// 		std::this_thread::sleep_for(std::chrono::milliseconds(10));
// 		current = measPT -> readInstr();
// 		disp = current - pos;  // distance from target
		
// 		// calculate direction
// 		if (disp > 0) direction = dir_down;
// 		else direction = dir_up;
		
// 		if (direction != prevDir) // if change direction 
// 		{
// 			//std::cout << "change!!" << std::endl;
// 			velocity = 0;
// 			sendVelocityToMicro();
// 			std::this_thread::sleep_for(std::chrono::milliseconds(10));
// 			sendCommandToMicro();
// 		}
// 		velocity = (unsigned int) (abs(disp) * k);
// 		//std::cout << velocity << std::endl;
// 		if (velocity != 0) sendVelocityToMicro();
		
// 		prevDir = direction;
// 	}
// 	velocity = 0;
// 	sendVelocityToMicro();
// 	lock = true;
// 	sendCommandToMicro();
// }

// void Asse::findMeasCenter()
// {
// 	float c = (measPT -> max + measPT -> min) / 2;
	
// 	startMove(startV, dir_down);
// 	float p;
// 	do {
// 		try
// 		{
// 			p = measPT -> readInstr();
// 		}
// 		catch (...)
// 		{
// 			p = std::numeric_limits<float>::infinity();
// 		}
// 	} while(p > c);
// 	stopMove();
// 	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
// 	setInstrPosition(c);
// }

void Asse::startMove(int v, bool d)
{
	lock = false;
	Asse::measuring = true;
	if (d) //decido la direzione del movimento
		direction = dir_fore;
	else
		direction = dir_back;

	velocity = v;
	sendVelocityToMicro();
}

void Asse::stopMove()
{
	Asse::measuring = false;
	
	velocity = 0;
	sendVelocityToMicro();
	lock = true;
}

Asse::~Asse()
{
	//ser.CloseSerialPort(); //lo distruggo nella seriale
}