#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>

#include "methods.h"
#include "CHRocodile.h"
#include "IkOptical.h"
#include "Moore.h"

void move(Moore& m);

int main()
{
	Moore moore;
	moore.init();
	
	move(moore);
	
	return 0;
}

void move(Moore& m)
{
	std::cout << std::fixed << std::setw(11) << std::setprecision(6) << std::setfill('0');
	
	m.moveX();
	m.moveY();
	
	pos p = m.getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
	

	float xpos, ypos, zpos;
	std::cout << "Where to go? x ";
	std::cin >> xpos;
	std::cout << "Where to go? y ";
	std::cin >> ypos;
	std::cout << "Where to go? z ";
	std::cin >> zpos;
	
	m.setCurrentPosition(xpos, ypos, zpos);
	
	p = m.getCurrentPosition();
	std::cout << "You are at: " << p.x << " " << p.y << " " << p.z << std::endl;
}