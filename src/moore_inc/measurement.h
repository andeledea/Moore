#pragma once
#include "sample.h"
#include "Moore.h"

#define APPROACH_LEFT false
#define APPROACH_RIGHT true

class Measurement
{
public:
    explicit Measurement() {};

    void setMooreSample(Moore* m, Sample* s);
    void approach(bool direction, double target, int speed);

    void setSafeLevel();
    
    ~Measurement() {};

private:
    Moore * moore;
    Sample * sample;

    double z_safe_level = NULL;
};

class SphereMeasurement : public Measurement
{
public:
    using Measurement::Measurement;

private:
    int n_repetitions;
};