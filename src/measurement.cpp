#include <cmath>
#include <cassert>
#include "measurement.h"

void Measurement::setMooreSample(Moore * m, Sample * s)
{
    this->moore = m;
    this->sample = s;
}

void Measurement::approach(bool direction, double target, int speed)
{
    this->reachSafeLevel();

    // move horizontally in x and y on top of the target
    pos target_sample = this->sample->getSideCoordinate(direction);

    target_sample.x = (direction == SIDE_LEFT) ? target_sample.x - 5 : target_sample.x + 5;
    target_sample.z = z_safe_level;
    this->moore->setAbsPosition(target_sample);

    // move down back to the sample height
    target_sample = this->sample->getSideCoordinate(direction);
    this->moore->Zaxis.setPosition(target_sample.z);

    // do the final approach
    this->moore->Xaxis.startMove(speed, direction);
    while(fabs(this->moore->cary.readInstr()) < target) {};
    this->moore->Xaxis.stopMove();
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
    assert(this->z_safe_level != NULL && "[ASSERTION_FAIL] z safe level must be set before approaching");
    // go up to avoid collisions
    this->moore->Zaxis.setPosition(z_safe_level);
}

///////////////// SPHERE MEAS ///////////////////////

void SphereMeasurement::setSamplePosition()
{
    std::cout << "[ACTION] Position the machine in contact with the sphere. Press ENTER";
    std::cin.ignore();
    pos contact_position = this->moore->getAbsPositionWithInstr();
    
    this->sample->setContactPosition(contact_position);
    std::cout << "[INFO] Sphere contact set to: " << contact_position << std::endl;
}
