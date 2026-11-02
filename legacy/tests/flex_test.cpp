#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>

#include "Moore.h"
#include "measurement.h"
#include "sample.h"

int main() {
    std::cout << "Simple Linear Regression Calculator - George Maier 2020 \n \n";
	
    //Data should be given in the form of arrays of float in the respective variables
    std::vector<double> datax = {3, 2, 4, 5, 3, 6, 2, 6, 5, 5, 4, 7, 8, 9, 10, 10, 7, 9, 12, 14, 16, 13, 12, 14, 16};
    std::vector<double> datay = {1, 3, 2, 4, 6, 4, 4, 6, 5, 3, 6, 8, 7, 10, 8, 12, 9, 9, 13, 12, 13, 13, 11, 12, 16};

    //Runs regression 
    slr regression(datax, datay);

    regression.print();
}

// int main(void)
// {
//     Moore moore;
//     BlockSample sample;
//     BlockMeasurement meas;
//     Probe probe(2.0, 20.0);

//     moore.init();
//     sample.setLength(10.0);

//     auto readAllInstr = [](Moore *m)
//     {
//         while (true)
//         {
//             m->updatePosition();
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         }
//     };
//     std::thread readT(readAllInstr, &moore);
//     readT.detach();

//     meas.setMooreSampleProbe(&moore, &sample, &probe);
//     meas.setSafeLevel();

//     meas.setSamplePosition();

//     meas.caryFlex(APPROACH_RIGHT);
    
//     return 0;
// }