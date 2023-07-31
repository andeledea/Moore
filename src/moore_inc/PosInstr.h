#pragma once
class PosInstr
{
public:
	explicit PosInstr() {
		value = 0;
		handle = -1;
	}

	virtual void connect() {}
	virtual void setParams() {}
	virtual double readInstr() { return 0.0; }

	~PosInstr() {}

	double value;
	double max;
	double min;
	int handle;
};

