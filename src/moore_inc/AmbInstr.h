#pragma once

/**
* Virtual class for ambiental instrument
* configuration and communication
* */
class AmbInstr
{
public:
	explicit AmbInstr() {}

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

	~AmbInstr() {}

	double value;
};
