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

pos SphereSample::getCenter()
{
    return this->center;
}

void SphereSample::setCenter(pos c)
{
    this->center = c;
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

//////////////////////// BLOCK /////////////////////

void BlockSample::setLength(double l)
{
    this->length = l;
}

double BlockSample::getLength()
{
    return this->length;
}

pos BlockSample::getSideCoordinate(bool direction)
{
    return (pos) {
        .x = (direction == SIDE_LEFT) ? this->contact_position.x - this->length : this->contact_position.x,
        .y = this->contact_position.y,
        .z = this->contact_position.z
    };
}

void BlockSample::setContactPosition(pos c)
{
    this->contact_position = c;
}
