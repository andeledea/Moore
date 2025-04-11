#pragma once
#include "sample.h"
#include "Moore.h"

#define APPROACH_LEFT false
#define APPROACH_RIGHT true

class Probe
{
public:
    Probe() {};
    Probe(double r, double l) : radius(r), length(l) {};

    double radius;
    double length;
};


class Measurement
{
public:
    explicit Measurement() {};

    void setMooreSampleProbe(Moore* m, Sample* s, Probe* p);
    void approach(bool direction, double target, int speed);
    void caryApproach(bool direction, double target, int speed);

    void setSafeLevel();
    void reachSafeLevel();

    virtual void setSamplePosition() {};
    
    ~Measurement() {};

protected:
    Moore * moore;
    Sample * sample;
    Probe * probe;

    double z_safe_level = std::numeric_limits<double>::infinity();
};

class BlockMeasurement : public Measurement
{
    public:
    using Measurement::Measurement;

    /**
     * The contact must be made to the right 
     */
    void setSamplePosition();

private:
    int n_repetitions; 
};

class SphereMeasurement : public Measurement
{
public:
    using Measurement::Measurement;

    /**
     * The contact must be made to the right 
     */
    void setSamplePosition();

private:
    int n_repetitions;
};