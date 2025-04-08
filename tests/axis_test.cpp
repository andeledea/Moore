// X axis test

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "Asse.h"
#include "IkOptical.h"
#include "SimpleSerial.h"
#include "keysight.h"

int main(void)
{
    std::cout << "axis test" << std::endl;
    SimpleSerial ser("COM7", CBR_57600);

    {
        Keysight laser;
        Asse xaxis;

        laser.connect();
        xaxis.init((PosInstr *)&laser, ser, x_lab);
        // acc, start, max, stop
        xaxis.setRamp(70000, 1000, 65535, 20);

        auto readlaser = [](Asse *axis)
        {
            while (true)
            {
                axis->getPosition();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        std::thread readT(readlaser, &xaxis);
        readT.detach();

        std::cout << "Going forward 5 mm from " << xaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        xaxis.setPosition(xaxis.getPosition() + 5);
        std::cout << "Arrived @ " << xaxis.getPosition() << std::endl;

        std::cout << "Going backward 5 mm from " << xaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        xaxis.setPosition(xaxis.getPosition() - 5);
        std::cout << "Arrived @ " << xaxis.getPosition() << std::endl;
    }

    IkOptical ik;
    {
        Scale yScale;
        Asse yaxis;

        // CONNECT THE OPTICAL SCALES
        ik.connect();
        yScale = ik.getYscale();
        yScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Y);
        yaxis.init((PosInstr *)&yScale, ser, y_lab);

        // acc, start, max, stop
        yaxis.setRamp(70000, 4000, 65535, 20, inv_mov);

        auto readlaser = [](Asse *axis)
        {
            while (true)
            {
                axis->getPosition();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        std::thread readT(readlaser, &yaxis);
        readT.detach();

        std::cout << "Going forward 5 mm from " << yaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        yaxis.setPosition(yaxis.getPosition() + 5);
        std::cout << "Arrived @ " << yaxis.getPosition() << std::endl;

        std::cout << "Going backward 5 mm from " << yaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        yaxis.setPosition(yaxis.getPosition() - 5);
        std::cout << "Arrived @ " << yaxis.getPosition() << std::endl;
    }

    {
        Scale zScale;
        Asse zaxis;

        // CONNECT THE OPTICAL SCALES
        ik.connect();
        zScale = ik.getZscale();
        zScale.setParams(ENC_INCREMENTAL, SIG_11UA, M_SIG_PERIOD_Z);
        zaxis.init((PosInstr *)&zScale, ser, z_lab);

        // acc, start, max, stop
        zaxis.setRamp(70000, 40000, 65535, 20000);

        auto readlaser = [](Asse *axis)
        {
            while (true)
            {
                axis->getPosition();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        std::thread readT(readlaser, &zaxis);
        readT.detach();

        std::cout << "Going forward 5 mm from " << zaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        zaxis.setPosition(zaxis.getPosition() + 5);
        std::cout << "Arrived @ " << zaxis.getPosition() << std::endl;

        std::cout << "Going backward 5 mm from " << zaxis.getPosition() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        zaxis.setPosition(zaxis.getPosition() - 5);
        std::cout << "Arrived @ " << zaxis.getPosition() << std::endl;
    }
    return 0;
}

/**
cd tests
g++ axis_test.cpp ..\src\IkOptical.cpp ..\src\Scale.cpp ..\src\SimpleSerial.cpp ..\src\Asse.cpp ..\src\keysight.cpp -I../src/moore_inc -L../lib/gcc_dll -lIK220Dll64 -o bin\axis_test
.\bin\axis_test.exe
*/