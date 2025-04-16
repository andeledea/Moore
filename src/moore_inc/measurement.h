#pragma once
#include <vector>

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

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
    virtual void setSpecializedSample() {};

    void approach(bool direction, double target, int speed);
    void caryApproach(bool direction, double target, int speed);
    void caryUnload(double distance=0.5);

    void setSafeLevel();
    void reachSafeLevel();

    /** @brief Function to find the max of a sample, the init position must be
     * approached before calling this function.
    * @param axis The axis of scan for the max
    * @return The max value on the scan axis
    */
    #define SCAN_SPEED 1000
    double findAxisMax(Asse *axis, double max_cary_disp, bool scan_dir);

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

    void setSpecializedSample() {this->block = (BlockSample*) this->sample;}

    /**
     * The contact must be made to the right 
     */
    void setSamplePosition();

private:
    BlockSample* block;
    int n_repetitions; 
};

class SphereMeasurement : public Measurement
{
public:
    using Measurement::Measurement;

    void setSpecializedSample() {this->sphere = (SphereSample*) this->sample;}

    /**
     * The contact must be made to the right 
     */
    void setSamplePosition();

    pos findMax(bool direction, double max_cary_disp=0.01);
    void findCenter(double max_cary_disp=0.01);

private:
    SphereSample * sphere;
    int n_repetitions;
};