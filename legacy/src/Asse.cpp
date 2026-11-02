#include <iostream>
#include <iomanip>
#include <thread>
#include <limits>
#include <chrono>
#include <cmath>
#include <conio.h>
#include "Asse.h"

bool Asse::measuring = false;
int Asse::keyboard_speed = MAX_KEY_SPEED * 0.1;

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

double Asse::getPositionWithInstr()
{
	if (this->measPT == nullptr) return this->getPosition();
    return this->getPosition() - this->measPT->preciseRead(5);
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

void Asse::timeBaseAccRamp(unsigned int minV, unsigned int maxV, bool d, int step)
{
	lock = false;

	if (d) direction = dir_fore;
	else direction = dir_back;

	velocity = minV;

	while (velocity <= maxV) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		sendVelocityToMicro();
		velocity += step;
	}
}

void Asse::timeBaseDecRamp(unsigned int minV, int step)
{
	while (velocity >= minV) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		sendVelocityToMicro();
		velocity -= step;
	}

	velocity = 0;
	lock = true;
	sendVelocityToMicro();
}

void Asse::keyboardControl(int forwardKey, int backwardKey)
{
	bool forwardPressed = false;
    bool backwardPressed = false;

    std::cout << "[INFO] Keyboard control enabled axis " << this->axisName << std::endl;

    while (true) {
		if (_kbhit()) {
			int key = _getch();
			
			if (key <= '9' && key >= '0') {
				switch (key) {
					case '0': keyboard_speed = MAX_KEY_SPEED; break;
					case '1': keyboard_speed = MAX_KEY_SPEED * 0.1; break;
					case '2': keyboard_speed = MAX_KEY_SPEED * 0.2; break;
					case '3': keyboard_speed = MAX_KEY_SPEED * 0.3; break;
					case '4': keyboard_speed = MAX_KEY_SPEED * 0.4; break;
					case '5': keyboard_speed = MAX_KEY_SPEED * 0.5; break;
					case '6': keyboard_speed = MAX_KEY_SPEED * 0.6; break;
					case '7': keyboard_speed = MAX_KEY_SPEED * 0.7; break;
					case '8': keyboard_speed = MAX_KEY_SPEED * 0.8; break;
					case '9': keyboard_speed = MAX_KEY_SPEED * 0.9; break;
				}
				std::cout << "[INFO] Ready with speed: " << std::setfill('-') << std::setw(6) << keyboard_speed << '\r';
			}

            // Check for forward key
            if (key == forwardKey && !backwardPressed) {
                if (!forwardPressed) {
                    timeBaseAccRamp(500, keyboard_speed, dir_fore, 20); // Call with direction forward
                    forwardPressed = true; // Mark as pressed
                } else {
                    timeBaseDecRamp(500, 20); // Call with direction forward
                    forwardPressed = false; // Mark as not pressed
                }
            }
            // Check for backward key
            else if (key == backwardKey && !forwardPressed) {
                if (!backwardPressed) {
                    timeBaseAccRamp(500, keyboard_speed, dir_back, 20); // Call with direction backward
                    backwardPressed = true; // Mark as pressed
                } else {
                    timeBaseDecRamp(500, 20); // Call with direction backward
                    backwardPressed = false; // Mark as not pressed
                }
            }
            // Exit on Escape key
            else if (key == 27) {
                std::cout << "[INFO] Keyboard control disabled" << std::endl;
                break;
            }
        }
    }
}

Asse::~Asse()
{
	//ser.CloseSerialPort(); //lo distruggo nella seriale
}