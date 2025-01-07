#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "keysight.h"

int main(void)
{
    Keysight ks;

    ks.connect();

    std::cout << "Starting KeySight test, press q for 1 second to close." << std::endl;
    while (true)
    {
        // Check if a key has been pressed
        if (_kbhit()) {
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') { // Check if it's 'q' or 'Q'
                break; // Exit the loop
            }
        }
        std::cout << "Laser position: " << ks.readInstr() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}

/**
cd tests
g++ keysight_test.cpp ..\src\keysight.cpp -I../src/moore_inc -L../lib/gcc_dll -lE1735A -lE1735ACore -o bin\keysight_test
.\bin\keysight_test.exe
*/