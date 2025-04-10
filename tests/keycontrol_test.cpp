#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Moore.h"

int main(void)
{
    Moore m;

    m.init();

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &m);
    readT.detach();

    m.keyboardMove();

    return 0;
}