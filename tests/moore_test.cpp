#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Moore.h"

int main(void)
{
    Moore m;

    m.init();

    std::cout << "[INFO] Starting moore test, press q for 1 second to close." << std::endl;

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &m);
    readT.detach();
    
    pos start = m.getAbsPosition();
    std::cout << "[INFO] Starting from: " << start << std::endl;

    pos end = start + (pos){ .x = 5, .y = 5, .z = 5};
    m.setAbsPosition(end);

    std::cout << "[INFO] Final position: " << m.getAbsPosition() << std::endl;
    std::cout << "[INFO] Going back to start" << std::endl;
    m.setAbsPosition(start);

    return 0;
}

/**
cd tests
g++ moore_test.cpp ../src/Moore.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll -lni4882 -lIK220Dll64 -lE1735A -lE1735ACore -o bin\moore_test
.\bin\moore_test.exe
*/