#pragma once
#include <vector>
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
    * @return
    */ 
    double findAxisMax(Asse *axis, double max_cary_disp=0.1) {
        std::vector<double> ax_pos, cary_pos;

        // Control that we are approached
        double cary = this->moore->cary.readInstr();
        if (abs(cary) < 0.01) throw std::runtime_error("[ERROR] Could not find max: the cary is not approached for max search.");

        bool back_to_fifty = false;

        auto carySafety = [axis, max_cary_disp](double val) {
            if (val > max_cary_disp * 1.5) {
                axis->timeBaseDecRamp(10);
                throw std::runtime_error("[ERROR] Cary max search safety limit reached.");
            } // safety check
        };

        axis->timeBaseAccRamp(10, 10, dir_back);
        while (cary > 0.01) {
            cary = abs(this->moore->cary.readInstr());
            carySafety(cary);
            ax_pos.emplace_back(axis->getPosition()); cary_pos.emplace_back(cary);
        }
        axis->timeBaseDecRamp(10);
        axis->timeBaseAccRamp(10, 10, dir_fore);
        while (cary > 0.01 || !back_to_fifty) {
            cary = abs(this->moore->cary.readInstr());
            if (cary > 0.05) back_to_fifty = true;
            carySafety(cary);
            ax_pos.emplace_back(axis->getPosition()); cary_pos.emplace_back(cary);
        }
        axis->timeBaseDecRamp(10);

        return 0.0; //TODO: calc the max position
    }

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

    void findMax(bool direction, double max_cary_disp=0.1) {
        // Go in contact to the best estimate
        this->approach(direction, 0.020, 800);

        // double half_chord = sqrtf(2 * this->sphere->getRadius() * max_cary_disp - max_cary_disp * max_cary_disp);
        this->findAxisMax(&this->moore->Yaxis, max_cary_disp);
        // this->findAxisMax(&this->moore->Zaxis, half_chord);
    }

private:
    SphereSample * sphere;
    int n_repetitions;
};