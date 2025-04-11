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
    SphereMeasurement meas;
    Probe probe(2.0, 20.0);

    moore.init();
    sample.setRadius(22.5);

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &moore);
    readT.detach();

    meas.setMooreSampleProbe(&moore, &sample, &probe);
    // meas.setSafeLevel();

    // meas.setSamplePosition();
    // meas.reachSafeLevel();
    
    // moore.keyboardMove();
    // meas.approach(APPROACH_RIGHT, 0.006, 1000);
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    
    meas.caryApproach(APPROACH_LEFT, 0.006, 1000);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}