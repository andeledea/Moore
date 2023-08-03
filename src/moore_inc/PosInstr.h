#pragma once

/**
* Virtual class for position instrument 
* configuration and communication
* */
class PosInstr
{
public:
	explicit PosInstr() {
		value = 0;
		handle = -1;
	}

	/**
	* Connects the instrument
	* */
	virtual void connect() {}

	/**
	* Sets the instrument parameters
	* */
	virtual void setParams() {}

	/**
	* Returns the instrument reading
	* 
	* @return value: the instrument reading
	* */
	virtual double readInstr() { return 0.0; }

	~PosInstr() {}

	double value;
	double max;
	double min;
	int handle;
};

