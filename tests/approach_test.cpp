#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Moore.h"
#include "measurement.h"
#include "sample.h"

int main(void)
{
    Moore moore;
    SphereSample sample;
    Measurement meas;

    moore.init();

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &moore);
    readT.detach();

    meas.setMooreSample(&moore, &sample);
    meas.setSafeLevel();

    return 0;
}