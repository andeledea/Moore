#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>

#include "methods.h"
#include "Asse.h"
#include "CHRocodile.h"
#include "IkOptical.h"

void connectMicro(SimpleSerial& ser);
void check(CHRocodile* CHR);
void lock_error(std::string err_msg);

Asse asseX, asseY; // variabili globali, MEH

int main()
{
	try
	{
		SimpleSerial ser;
		connectMicro(ser); // connessione per entrambi gli assi

		CHRocodile CHR;
		Laser las;
		IkOptical scale;

		asseX.init((PosInstr*) &las, ser, 'X');
		asseY.init((PosInstr*) &CHR, ser, 'Y');

		asseX.setRamp(1000, 25, 255, 15);
		asseY.setRamp(2.0f, 180, 200, 5, inv_mov);

		char com;
		do
		{
			std::cout << "m: move, s: start measure, o: optical, q: quit" << std::endl;
			std::cin >> com;
			switch (com)
			{
			case 'o':
				measScale(scale);
				break;
			case 'm':
				std::cout << "Keep range control during movement? (y/n) ";
				std::cin >> com;
				if (com == 'y')
				{
					std::thread checkCHR(check, &CHR); // avvio il controllo 
					move(asseX);
					checkCHR.detach();
				}
				else move(asseX);
				break;
			case 's':
				std::thread checkCHR(check, &CHR); // avvio il controllo 
				measure(asseX, asseY, las, CHR);
				checkCHR.detach();
				break;
			}
		} while (com != 'q');
	}
	catch (std::runtime_error & _e)
	{
		lock_error(_e.what());
	}
}

void connectMicro(SimpleSerial& ser)
{
	char port[] = "COM5";
	if (ser.OpenSerialPort(port, CBR_57600)) // apro la com seriale
	{
		std::cout << "Asse connesso" << std::endl;
	}
	else
	{
		throw std::runtime_error("Unable to connect micro");
	}
}

void check(CHRocodile* CHR)
{
	while (true)
	{
		while (CHR->cnt_check > 0)
		{
			Sleep(50);
			CHR->cnt_check--;
		}
		try
		{
			CHR->readInstr();
			// std::cout << "check OK" << std::endl;
		}
		catch (std::runtime_error & _e)
		{
			lock_error(_e.what());
			exit(-2);
		}
	}
}

void lock_error(std::string err_msg)
{
	std::cerr << "Terminating: " << err_msg << std::endl;
	asseX.stopMeasure(); //fermo i motori e li blocco
	asseY.stopMeasure();
}