#pragma once

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
	friend std::ostream& operator<<(std::ostream&, const pos&);
	friend pos operator+(pos l, const pos& r);
	friend pos operator-(pos l, const pos& r);
};