<<<<<<< HEAD
#include "app.h"
#include "Moore.h"
#include "PosFrame.h"
#include "CHRMeasFrame.h"

// implement the application
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    MyApp::online = true;
=======
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>

#include "methods.h"
#include "Asse.h"
#include "CHRocodile.h"
#include "IkOptical.h"
#include "Moore.h"

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
		IkOptical scales;
>>>>>>> ab36b7f (Added moore class for movement)

    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    MooreCHRMeasFrame* CHRmeasFrame = new MooreCHRMeasFrame(nullptr);
    
    posFrame->Show(true);
    CHRmeasFrame->Show(true);

	Moore* moore = new Moore();
    moore->init();
	
    posFrame->setMoore(moore);  // starts the read thread
    CHRmeasFrame->setMoore(moore);

<<<<<<< HEAD
    return true; 
=======
		char com;
		do
		{
			std::cout << "m: move, s: start measure, o: optical, q: quit" << std::endl;
			std::cin >> com;
			switch (com)
			{
			case 'o':
				measScale(scales);
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
>>>>>>> ab36b7f (Added moore class for movement)
}

int MyApp::OnExit()
{
    MyApp::online = false;
    return 0;
}

bool MyApp::online = false;
 