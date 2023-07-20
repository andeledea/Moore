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
	virtual float readInstr() { return 0.0f; }

	~PosInstr() {}

	float value;
	float max;
	float min;
	int handle;
};

