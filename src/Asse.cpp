#include <iostream>
#include <iomanip>
#include "Asse.h"

Asse::Asse()
{
	lockB = true;
}

Asse::Asse(PosInstr *instrument, SimpleSerial &serial, char name)
{
	lockB = true;
	
	ser = serial;
	instrPT = instrument;

	instrPT -> connect();
	instrPT -> setParams();
	// position = instrPT -> readInstr();

	axisName = name;
}

void Asse::init(PosInstr* instrument, SimpleSerial& serial, char name)
{
	ser = serial;
	instrPT = instrument;
	axisName = name;

	instrPT->connect();
	instrPT->setParams();
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
			this->setDirectionForeward();
		else
			this->setDirectionBackward();
	}
	else
	{
		if (startP < targetPosition) //decido la direzione del movimento
			this->setDirectionBackward();
		else
			this->setDirectionForeward();
	}
	
	this -> unlock(); //sblocco per il movimento

	this->sendToMicro("V");

	float distance = abs(targetPosition - startP); //calcolo la dist totale
	float travel = 0;

	while (distance - travel > 0) // finchè non raggiungo il target
	{
		setVelocity(distance, travel); //imposto la velocità istantanea
		travel = abs(instrPT->readInstr() - startP); //calcolo il percorso fatto
	};
	
	char s = 0;
	ser.WriteSerialPort(&s, 1); // termino l'invio di velocità

	this -> velocity = 0;
	this -> lock(); // riblocco l'asse
}

void Asse::setRamp(float acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement)
{
	acceleration = acc;
	startV = startv;
	maxV = maxv;
	stopV = stopv;

	invertedMovement = invertMovement;
}

bool Asse::isLocked()
{
	return lockB;
}

void Asse::lock()
{
	lockB = true;
	this->sendToMicro("L");
}

void Asse::unlock()
{
	lockB = false;
	this->sendToMicro("U");
}



Asse::~Asse()
{
	//ser.CloseSerialPort(); //lo distruggo nella seriale
}

// private methods

/* Calcola la velocità istantanea seguendo la rampa di
   acc e dec. Con il parametro ramp e acc si possono settare
   durata e pendenza della rampa 
   DIST: total distance
   TRAV: distance already done*/
void Asse::setVelocity(float dist, float trav)
{
	unsigned int oldV = velocity;
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
	
	if (velocity != oldV)
	{
		char mapV = (char)velocity; // map della velocità
	
		char send[3] = {axisName, mapV};
		ser.WriteSerialPort(send);
	}
}

void Asse::sendToMicro(std::string toSend)
{
	toSend = axisName + toSend; //indico il nome dell'asse da muovere al micro
	std::cout << toSend;
	ser.WriteSerialPort((char*)toSend.c_str());
	std::cout << ser.ReadSerialPort(1, "square") << std::endl;
}

void Asse::setDirectionForeward()
{
	this -> sendToMicro("F");
}

void Asse::setDirectionBackward()
{
	this->sendToMicro("B");
}

void Asse::retension(float pos, int v)
{
	this->startMeasure(v, dir_up);
	while(instrPT -> readInstr() < pos) {}
	this->stopMeasure();
}

void Asse::startMeasure(int v, bool d)
{
	this -> unlock();
	if (d) //decido la direzione del movimento
		this->setDirectionForeward();
	else
		this->setDirectionBackward();
	
	this->sendToMicro("V");

	char val = (unsigned char)v;
	ser.WriteSerialPort(&val, 1); //parto a velocità per la misura
}

void Asse::stopMeasure()
{
	char s = 0;
	ser.WriteSerialPort(&s, 1); // termino l'invio di velocità
	this->lock();
}
