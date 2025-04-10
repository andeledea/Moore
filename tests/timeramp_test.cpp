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
    std::cout << "[INFO] axis test" << std::endl;
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

        xaxis.timeBaseAccRamp(500, 40000, dir_fore, 20);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        xaxis.timeBaseDecRamp(500, 40000, dir_fore, 20);
    }
}