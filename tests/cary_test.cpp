#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "cary.h"

int main(void)
{
    Cary cary;

    cary.connect();
    // cary.setRange(RNG_50);

    std::cout << "[INFO] Starting cary test, press q for 1 second to close." << std::endl;
    while (true)
    {
        // Check if a key has been pressed
        if (_kbhit()) {
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') { // Check if it's 'q' or 'Q'
                break; // Exit the loop
            }
        }
        std::cout << "[INFO] Cary reading: " << cary.readInstr() << '\r';
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}

/**
cd tests
g++ cary_test.cpp ..\src\cary.cpp ..\src\SimpleSerial.cpp -I../src/moore_inc -o bin\cary_test
.\bin\cary_test.exe
*/