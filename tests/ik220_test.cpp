#include <iostream>
#include <thread>
#include <chrono>
#include<conio.h>
#include "IkOptical.h"

int main(void)
{
    IkOptical ik;

    ik.connect();
    ik.setParams();

    std::cout << "[INFO] Starting ik220 test, press q for 1 second to close." << std::endl;
    while (true)
    {
        // Check if a key has been pressed
        if (_kbhit()) {
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') { // Check if it's 'q' or 'Q'
                break; // Exit the loop
            }
        }
        ik.readInstr();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}

/**
cd tests
g++ ik220_test.cpp ..\src\IkOptical.cpp ..\src\Scale.cpp -I../src/moore_inc -L../lib/gcc_dll -lIK220Dll64 -o bin\ik220_test
.\bin\ik220_test.exe
*/