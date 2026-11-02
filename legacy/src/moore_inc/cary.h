#pragma once
#include <iostream>

#include "SimpleSerial.h"
#include "PosInstr.h"

enum {
    RNG_5000,
    RNG_2000,
    RNG_500,
    RNG_200,
    RNG_50,
    RNG_20,
    RNG_5,
    RNG_AUTO,
};

class Cary : PosInstr 
{
public:
    // using PosInstr :: PosInstr;
    Cary();

    void connect();
    double readInstr();
    double preciseRead(int n_samples_to_read);
    int getRange();
    void setRange(int rng);

    double getValue() { return value; };

    ~Cary();
    static int faults;
    static std::chrono::_V2::system_clock::time_point start;
    
private:
    SimpleSerial ser;

};