#include <iostream>
#include <iomanip>
#include "pos.h"

// redefine operator to print positions
std::ostream& operator<<(std::ostream& os, const pos& p)
{
	os << p.x << " " << p.y << " " << p.z << std::endl;
	return os;
}

pos operator+(pos l, const pos& r)
{
	l += r;
	return l;
}

pos operator-(pos l, const pos& r)
{
	l -= r;
	return l;
}