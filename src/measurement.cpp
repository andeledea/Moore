#include <cmath>
#include <cassert>
#include <iomanip>
#include "measurement.h"

void Measurement::setMooreSampleProbe(Moore * m, Sample * s, Probe* p)
{
    this->moore = m;
    this->sample = s;
    this->probe = p;
}

void Measurement::approach(bool direction, double target, int speed)
{
    this->reachSafeLevel();

    // move horizontally in x and y on top of the target
    pos target_sample = this->sample->getSideCoordinate(direction);

    target_sample.x = (direction == SIDE_LEFT) ? target_sample.x - probe->radius - 0.5 : target_sample.x + probe->radius + 0.5;
    target_sample.z = z_safe_level;
    this->moore->setAbsPosition(target_sample);

    // move down back to the sample height
    target_sample = this->sample->getSideCoordinate(direction);
    this->moore->Zaxis.setPosition(target_sample.z);

    this->caryApproach(direction, target, speed);
}

void Measurement::caryApproach(bool direction, double target, int speed)
{
    this->moore->Xaxis.timeBaseAccRamp(speed, speed, !direction);

    double cary = abs(this->moore->cary.preciseRead(3));
    while (cary < target) {
        cary = abs(this->moore->cary.preciseRead(3));
        std::cout << "[INFO] Cary approach: " << std::setfill('#') << std::setw(6) << cary << '/' << target << '\r';
    }

    this->moore->Xaxis.timeBaseDecRamp(speed);
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
