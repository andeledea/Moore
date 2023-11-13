#include <iostream>
#include <stdexcept>

#include "E1735A.h"
#include "keysight.h"

void KeySight::connect()
{
    int status = Initialize_E1735A_DLL();
    if (status == 0) {
        std::cout << "The E1735A.dll was initialized successfully\n";
        if((E1735A_ReadDeviceCount() > 0) && E1735A_SelectDevice(0)) {
            //E1735A_ResetDevice();
            std::cout << "The e1735a was found and selected successfully\n";
        } else {
            throw std::runtime_error("The e1735a was not found and selected successfully\n");
        }
	} else throw std::runtime_error("Failed to initialize E1735A.dll\n");
}

void KeySight::setParams()
{
    for(int i = 0; i < 5; i++)
    {
        E1735A_BlinkLED();
    }
    /* Parameter Settings
    E1735A_SetSampleTriggers(TB_SOFTWARE);  // TB_TIMER, TB_REMOTE, TB_AQB, TB_EXTERNAL
    E1735A_SetupTimer(0.01);  // 10 ms
    if (!E1735A_StartTimer()) throw std::runtime_error("Could not start E1735A Timer");
    E1735A_SetOptics(0);
    E1735A_SetParameter(0, 632.991354);  // wavelength
    E1735A_SetParameter(1, 20.0);  // air Temp ...
    */
}

double KeySight::readInstr()
{
    double sample;
	if(sample =  E1735A_ReadSample()) { 
		std::cout << "Laser Sample = " << sample << "mm\n";
		return sample; 
	}
    else {
		std::cout << "Error = " << E1735A_ReadLastError() << std::endl;
		std::cout << "Trigger = " << E1735A_ReadLastTrigger() << std::endl;	
	}
}

KeySight::~KeySight()
{
    int status = Finalize_E1735A_DLL();
	if (status == 0) std::cout << "The E1735A.dll was ended successfully\n"; 
    else throw std::runtime_error("Failed to end E1735A.dll\n");
}
