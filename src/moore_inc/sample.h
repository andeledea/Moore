#pragma once
#include <string>
#include "pos.h"

#define SIDE_LEFT false
#define SIDE_RIGHT true

class Sample
{
public:
    explicit Sample()
    {
        this->id = "";
        this->name = "";
    };

    void setName(std::string name);
    void setId(std::string id);

    virtual pos getSideCoordinate(bool direction) {};
    virtual void setCenterPosition();

    ~Sample() {};

private:
    std::string name;
    std::string id;
};

class SphereSample : public Sample
{
public:
    using Sample::Sample;

    void setRadius(double r);
    double getRadius();

    pos getSideCoordinate(bool direction);

private:
    double radius;
    pos center;
};