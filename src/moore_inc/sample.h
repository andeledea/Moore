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

class BlockSample : public Sample
{
public:
    using Sample::Sample;
    
    void setLength(double l);
    double getLength();

    pos getSideCoordinate(bool direction);
    void setContactPosition(pos c);

private:
    double length;
};

class SphereSample : public Sample
{
public:
    using Sample::Sample;

    void setRadius(double r);
    double getRadius();

    pos getCenter();
    void setCenter(pos c);

    pos getSideCoordinate(bool direction);
    void setContactPosition(pos c);

private:
    double radius;
    pos center;
};