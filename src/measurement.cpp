#include <cmath>
#include <cassert>
#include <iomanip>
#include "measurement.h"

double averageFromVectors(std::vector<double> &x, std::vector<double> &y)
{
    namespace plt = matplotlibcpp;
    plt::plot(x, y, "-r");

    std::vector<int> indices(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        indices[i] = i;
    }
    // Sort indices based on the values in vec1
    std::sort(indices.begin(), indices.end(), [&y](int a, int b)
              { return y[a] > y[b]; });

    // TODO average all symmetrical max
    int i = 0;
    int n_max_points = 1;
    double max_avg = x[indices[i]];
    while (y[indices[i]] == y[indices[i + 1]])
    {
        max_avg += x[indices[i + 1]];
        n_max_points++;
        i++;
        if (i == indices.size())
            break;
    }
    max_avg /= n_max_points;

    plt::axvline(max_avg);
    std::cout << "[INFO] Found max at " << max_avg << std::endl;

    plt::xlabel("Scan axis position [mm]");
    plt::ylabel("Scan cary position [mm]");
    plt::show();

    return max_avg;
}

void Measurement::setMooreSampleProbe(Moore *m, Sample *s, Probe *p)
{
    this->moore = m;
    this->sample = s;
    this->probe = p;

    this->setSpecializedSample();
}

void Measurement::approach(bool direction, double target, int speed)
{
    this->reachSafeLevel();

    // move horizontally in x and y on top of the target
    pos target_sample = this->sample->getSideCoordinate(direction);

    target_sample.x = (direction == SIDE_LEFT) ? target_sample.x - probe->radius - 0.1 : target_sample.x + probe->radius + 0.1;
    target_sample.z = z_safe_level;
    this->moore->setAbsPosition(target_sample);

    // move down back to the sample height
    target_sample = this->sample->getSideCoordinate(direction);
    this->moore->Zaxis.setPosition(target_sample.z);

    this->caryApproach(direction, target, speed);
}

void Measurement::caryApproach(bool direction, double target, int speed)
{
    // TODO: dosent work when called fom approach LEFT (probably cary value read is wrong)
    // maybe we need to flush the serial after eac approach?? (purgePort())
    this->moore->Xaxis.timeBaseAccRamp(speed, speed, !direction);

    double cary = abs(this->moore->cary.readInstr());
    std::cout << "[INFO] Cary approach started" << std::endl;
    while (cary < target)
    {
        if (cary > 0.002)
            this->moore->Xaxis.timeBaseAccRamp(15, 15, !direction);
        cary = abs(this->moore->cary.readInstr());
        std::cout << "[INFO] Cary approach: " << cary << '\r';
    }

    this->moore->Xaxis.timeBaseDecRamp(speed);
}

void Measurement::caryUnload(double distance)
{
    double cary = this->moore->cary.readInstr();
    if (abs(cary) < 0.001)
        return; // the cary is not approached
    double pos = this->moore->Xaxis.getPosition();
    if (cary > 0)
        this->moore->Xaxis.setPosition(pos - distance);
    if (cary < 0)
        this->moore->Xaxis.setPosition(pos + distance);
    if (abs(cary) > 0.001)
        this->caryUnload(distance); // the dist was not enough
}

void Measurement::setSafeLevel()
{
    std::cout << "[ACTION] Move the Z axis clear from any collision. Press ENTER";
    std::cin.ignore();
    this->z_safe_level = this->moore->getAbsPosition().z;
    std::cout << "[INFO] Z safe level set to: " << this->z_safe_level << std::endl;
}

void Measurement::reachSafeLevel()
{
    assert(this->z_safe_level < std::numeric_limits<double>::infinity() && "[ASSERTION_FAIL] z safe level must be set before approaching");
    // go up to avoid collisions
    this->moore->Zaxis.setPosition(z_safe_level);
}

double Measurement::findAxisMax(Asse *axis, double max_cary_disp, bool scan_dir)
{
    std::vector<double> ax_pos, cary_pos;

    // Control that we are approached
    double cary = abs(this->moore->cary.readInstr());
    if (cary < 0.7 * max_cary_disp)
        throw std::runtime_error("[ERROR] Could not find max: the cary is not approached for max search.");

    bool back_to_fifty = false;

    auto carySafety = [axis, max_cary_disp](double val)
    {
        if (val > max_cary_disp * 1.5)
        {
            axis->timeBaseDecRamp(SCAN_SPEED);
            throw std::runtime_error("[ERROR] Cary max search safety limit reached.");
        } // safety check
    };

    axis->timeBaseAccRamp(SCAN_SPEED, SCAN_SPEED, !scan_dir);
    while (cary > 0.8 * max_cary_disp)
    {
        std::cout << "[INFO] Finding start: " << cary << '\r';
        cary = abs(this->moore->cary.readInstr());
        carySafety(cary);
    }
    axis->timeBaseDecRamp(SCAN_SPEED);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    axis->timeBaseAccRamp(SCAN_SPEED, SCAN_SPEED, scan_dir);
    while (cary > 0.8 * max_cary_disp || !back_to_fifty)
    {
        std::cout << "[INFO] Scanning axis: " << cary << '\r';
        cary = abs(this->moore->cary.readInstr());
        if (cary > 0.9 * max_cary_disp && !back_to_fifty)
        {
            back_to_fifty = true;
        }
        carySafety(cary);
        ax_pos.emplace_back(axis->getPosition());
        cary_pos.emplace_back(cary);
    }
    axis->timeBaseDecRamp(SCAN_SPEED);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return averageFromVectors(ax_pos, cary_pos);
}

///////////////// SPHERE MEAS ///////////////////////

void SphereMeasurement::setSamplePosition()
{
    std::cout << "[ACTION] Position the machine in contact with the sphere. ESC + ESC to end" << std::endl;
    this->moore->keyboardMove();

    pos contact_position = this->moore->getAbsPositionWithInstr();
    contact_position.x -= this->probe->radius;

    this->sample->setContactPosition(contact_position);
    std::cout << "[INFO] Sphere contact set to: " << contact_position << std::endl;
}

pos SphereMeasurement::findMax(bool direction, double max_cary_disp)
{
    // double half_chord = sqrtf(2 * this->sphere->getRadius() * max_cary_disp - max_cary_disp * max_cary_disp);

    // Go in contact to the best estimate
    double y_center = 0.0;
    this->approach(direction, max_cary_disp, 800);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    y_center += this->findAxisMax(&this->moore->Yaxis, max_cary_disp, dir_back);
    this->caryUnload();
    
    this->approach(direction, max_cary_disp, 800);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    y_center += this->findAxisMax(&this->moore->Yaxis, max_cary_disp, dir_fore);
    this->caryUnload();
    
    y_center /= 2;
    
    double z_center = 0.0;
    this->approach(direction, max_cary_disp, 800);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    z_center += this->findAxisMax(&this->moore->Zaxis, max_cary_disp, dir_up);
    this->caryUnload();
    
    this->approach(direction, max_cary_disp, 800);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    z_center += this->findAxisMax(&this->moore->Zaxis, max_cary_disp, dir_down);
    this->caryUnload();
    
    z_center /= 2;

    return (pos){ .x = 0, .y = y_center, .z = z_center };
}

void SphereMeasurement::findCenter(double max_cary_disp)
{
    // TODO: test the whole thing
    pos pos_right = this->findMax(APPROACH_RIGHT, max_cary_disp);
    pos pos_left = this->findMax(APPROACH_LEFT, max_cary_disp);

    pos center = (pos_left + pos_right) / 2.0;
    center.x = this->sphere->getCenter().x;  // use init estimate for x center position

    std::cout << "[INFO] Sphere center refined to: " << center << std::endl;
    this->sphere->setCenter(center);
}

///////////////// BLOCK MEAS ///////////////////////

void BlockMeasurement::setSamplePosition()
{
    std::cout << "[ACTION] Position the machine in contact with the block. ESC + ESC to end" << std::endl;
    this->moore->keyboardMove();

    pos contact_position = this->moore->getAbsPositionWithInstr();
    contact_position.x -= this->probe->radius;

    this->sample->setContactPosition(contact_position);
    std::cout << "[INFO] Block contact set to: " << contact_position << std::endl;
}
