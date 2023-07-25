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
	
	char c;
	while(true)
	{
		std::cin >> c;
		switch (c)
		{
			case 'z':
				moore.defineZero();
				break;
			case 'm':
				move(moore);
				break;
			case 's':
				moore.measCHR();
				break;
			case 'i':
				moore.moveInstr();
				break;
			case 'w':
				std::cout << "REL: " << moore.getRelPosition();
				std::cout << "ABS: " << moore.getAbsPosition();
				break;
		}
	}
	
	return 0;
}

void move(Moore& m)
{
	std::cout << std::fixed << std::setw(11) << std::setprecision(6) << std::setfill('0');
	
	std::cout << "REL: " << m.getRelPosition();
	std::cout << "ABS: " << m.getAbsPosition();
	
	char v;
	std::cout << "a/r? ";
	std::cin >> v;

	pos target;
	std::cout << "Where to go? x ";
	std::cin >> target.x;
	std::cout << "Where to go? y ";
	std::cin >> target.y;
	std::cout << "Where to go? z ";
	std::cin >> target.z;
	
	switch(v)
	{
		case 'r':
			std::cout << "Setting relative position" << std::endl;
			m.setRelPosition(target);
			break;
		case 'a':
			std::cout << "Setting absolute position" << std::endl;
			m.setAbsPosition(target);
			break;
	}
}