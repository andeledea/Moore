#include <iostream>

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
            E1735A_SetSampleTriggers(TB_SOFTWARE);
        } else {
            std::cout << "The e1735a was not found and selected successfully\n";
        }
	} else std::cout << "Failed to initialize E1735A.dll\n";

}

void KeySight::setParams()
{
    E1735A_BlinkLED();
}

double KeySight::readInstr()
{
   double sample;   
	//1735A_ResetDevice();
    bool status;
	//status = E1735A_SetSampleTriggers(TB_SOFTWARE);
	//printf("status %d", status);	
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
    else std::cout << "Failed to end E1735A.dll\n";
}
