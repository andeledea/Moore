#include <iostream>
#include <thread>
#include <chrono>
#include "IkOptical.h"

int main(void)
{
    IkOptical ik;

    ik.connect();
    ik.setParams();

    std::cout << "Starting ik220 test" << std::endl;
    while (true)
    {
        ik.readInstr();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// g++ ik220_test.cpp ..\src\IkOptical.cpp ..\src\Scale.cpp -I../src/moore_inc -L../lib/gcc_dll -lIK220Dll64 -o bin\ik220_test
// .\bin\ik220_test.exe