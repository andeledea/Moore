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
    * @return
    */
    #define SCAN_SPEED 1500
    double findAxisMax(Asse *axis, double max_cary_disp) {
        std::vector<double> ax_pos, cary_pos;

        // Control that we are approached
        double cary = abs(this->moore->cary.readInstr());
        if (cary < 0.7 * max_cary_disp) throw std::runtime_error("[ERROR] Could not find max: the cary is not approached for max search.");

        bool back_to_fifty = false;

        auto carySafety = [axis, max_cary_disp](double val) {
            if (val > max_cary_disp * 1.5) {
                axis->timeBaseDecRamp(SCAN_SPEED);
                throw std::runtime_error("[ERROR] Cary max search safety limit reached.");
            } // safety check
        };

        axis->timeBaseAccRamp(SCAN_SPEED, SCAN_SPEED, dir_back);
        while (cary > 0.8 * max_cary_disp) {
            std::cout << "[INFO] Scanning back: " << cary << '\r';
            cary = abs(this->moore->cary.readInstr());
            carySafety(cary);
        }
        axis->timeBaseDecRamp(SCAN_SPEED);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        axis->timeBaseAccRamp(SCAN_SPEED, SCAN_SPEED, dir_fore);
        while (cary > 0.8 * max_cary_disp || !back_to_fifty) {
            std::cout << "[INFO] Scanning fore: " << cary << '\r';
            cary = abs(this->moore->cary.readInstr());
            if (cary > 0.9 * max_cary_disp && !back_to_fifty) 
            {
                std::cout << "[INFO] BACK TO FIFTY" << cary << std::endl;
                back_to_fifty = true;
            }
            carySafety(cary);
            ax_pos.emplace_back(axis->getPosition()); cary_pos.emplace_back(cary);
        }
        axis->timeBaseDecRamp(SCAN_SPEED);
        std::this_thread::sleep_for(std::chrono::seconds(2));

        namespace plt = matplotlibcpp;
        plt::plot(ax_pos, cary_pos, "-r");
        
        std::vector<int> indices(ax_pos.size());
        for (int i = 0; i < ax_pos.size(); ++i) {
            indices[i] = i;
        }
        // Sort indices based on the values in vec1
        std::sort(indices.begin(), indices.end(), [&cary_pos](int a, int b) { return cary_pos[a] > cary_pos[b]; });

        int i = 1;
        double sum = ax_pos[indices[0]];
        while(cary_pos[indices[i-1]] == cary_pos[indices[i]]) {
            sum += ax_pos[indices[i]];
            i++;
            std::cout << "[INFO] Found " << i << " max as " << ax_pos[indices[i]] << " : " << cary_pos[indices[i]] << '\r';
        }
        double max_position = sum / i;
        std::cout << "[INFO] Found max at " << max_position << std::endl;

        plt::axvline(max_position);
        plt::xlabel("Scan axis position [mm]");
        plt::ylabel("Scan cary position [mm]");
        plt::show();

        return max_position;
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

    void findMax(bool direction, double max_cary_disp=0.01) {
        // Go in contact to the best estimate
        this->approach(direction, max_cary_disp, 800);

        // double half_chord = sqrtf(2 * this->sphere->getRadius() * max_cary_disp - max_cary_disp * max_cary_disp);
        this->findAxisMax(&this->moore->Yaxis, max_cary_disp);
        // this->findAxisMax(&this->moore->Zaxis, half_chord);
    }

private:
    SphereSample * sphere;
    int n_repetitions;
};