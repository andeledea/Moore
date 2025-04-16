#pragma once

/**
* 3D position container
* 
* provides simple math operations on 3D vectors and defines a position
* in the machine space
* */
class pos // defines a reference system (only translation no rotation)
{
public:
	double x = 0;
	double y = 0;
	double z = 0;

	pos& operator-=(const pos& lr)
	{
		this->x -= lr.x;
		this->y -= lr.y;
		this->z -= lr.z;

		return *this;
	}
	pos& operator+=(const pos& lr)
	{
		this->x += lr.x;
		this->y += lr.y;
		this->z += lr.z;

		return *this;
	}
	pos& operator*=(const double& lr)
	{
		this->x *= lr;
		this->y *= lr;
		this->z *= lr;

		return *this;
	}
	pos& operator/=(const double& lr)
	{
		this->x /= lr;
		this->y /= lr;
		this->z /= lr;

		return *this;
	}
	friend std::ostream& operator<<(std::ostream&, const pos&);
	friend pos operator+(pos l, const pos& r);
	friend pos operator-(pos l, const pos& r);
	friend pos operator*(pos l, const double& r);
	friend pos operator/(pos l, const double& r);
};