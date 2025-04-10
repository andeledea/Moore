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

    virtual pos getSideCoordinate(bool direction) { return (pos) {}; };
    virtual void setContactPosition(pos c) {};

    ~Sample() {};
    
protected:
    std::string name;
    std::string id;

    pos contact_position;
};

class SphereSample : public Sample
{
public:
    using Sample::Sample;

    void setRadius(double r);
    double getRadius();

    void setContactPosition(pos c);

    pos getSideCoordinate(bool direction);

private:
    double radius;
    pos center;
};