#include <iostream>
#include <thread>
#include <chrono>
#include "keysight.h"

int main(void)
{
    KeySight ks;

    ks.connect();
    ks.setParams();

    std::cout << "Starting KeySight test" << std::endl;
    while (true)
    {
        ks.readInstr();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// g++ keysight_test.cpp ..\src\keysight.cpp -I../src/moore_inc -L../lib/gcc_dll -lE1735A -lE1735ACore -o bin\keysight_test
// .\bin\keysight_test.exe