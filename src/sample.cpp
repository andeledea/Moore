#include "sample.h"

void Sample::setName(std::string name)
{
    this->name = name;
}

void Sample::setId(std::string id)
{
    this->id = id;
}

////////////////// SPHERE ///////////////

void SphereSample::setRadius(double r)
{
    this->radius = r;
}

double SphereSample::getRadius()
{
    return this->radius;
}

void SphereSample::setContactPosition(pos c)
{
    this->contact_position = c;
    this->center = c - (pos) {.x = this->radius, .y = 0, .z = 0};
}

pos SphereSample::getSideCoordinate(bool direction)
{
    return (pos) {
        .x = (direction == SIDE_LEFT) ? this->center.x - this->radius : this->center.x + this->radius,
        .y = this->center.y,
        .z = this->center.z
    };
}
