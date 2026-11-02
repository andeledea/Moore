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
    BlockSample sample;
    BlockMeasurement meas;
    Probe probe(2.0, 20.0);

    moore.init();
    sample.setLength(10.0);

    auto readAllInstr = [](Moore *m)
    {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &moore);
    readT.detach();

    meas.setMooreSampleProbe(&moore, &sample, &probe);
    meas.setSafeLevel();

    meas.setSamplePosition();
    meas.reachSafeLevel();

    // moore.keyboardMove();
    for (int i = 0; i < 1; i++)
    {
        meas.approach(APPROACH_RIGHT, 0.006, 800);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        meas.caryUnload();
        
        meas.approach(APPROACH_LEFT, 0.006, 800);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        meas.caryUnload();
    }
    return 0;
}