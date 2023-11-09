#pragma once
#include <thread>
#include <memory>
#include "SimpleSerial.h"
#include "PosInstr.h"

#define dir_back false
#define dir_fore true

#define dir_up false
#define dir_down true

#define inv_mov true

#define x_lab 0b00000000
#define y_lab 0b01000000
#define z_lab 0b10000000


/**
* Class control for the single axis
* */
class Asse
{
public:
	Asse();
	Asse(PosInstr* instrument, SimpleSerial& serial, char name);

	/**
	* Initialises the axis object
	* 
	* @param instrument: encoder relative to the axis
	* @param serial: the serial connection for motor control
	* @param name: the byte that identifies the axis
	* */
	void init(PosInstr* instrument, SimpleSerial& serial, char name);
	
	/**
	* If the axis is equipped with an external
	* instrument, the reading can be done by setting
	* the instrument with this method.
	* 
	* @param instrument: The external position instument 
	* */
	void setMeasInstrument(PosInstr* instrument) { measPT = instrument; }

	/**
	* If an external instrument is set it is possible
	* to track the sample profile keeping the instrument
	* always at the same distance
	* 
	* @param pos: The position that must be kept by the external instrument
	* */
	void track(float pos);

	/**
	* If an external instrument is set this method
	* allows to find the axis position that centers
	* the instrument reading
	* */
	void findMeasCenter();

	/**
	* Set the position of the axis relative to
	* the external instrument
	* 
	* @param targetPos: The position that has to be reached
	* */
	void setInstrPosition(float targetPos) { setPos(targetPos, 'm'); }

	/**
	* Reads the absolute encoder value
	* 
	* @return Position: The absolute position of the axis
	* */
	double getPosition(); //ritorna la pos attuale

	/**
	* Set the position of the axis relative
	* to the encoder position
	* 
	* @param targetPos: The position that has to be reached
	* */
	void setPosition(double targetPos) { setPos(targetPos, 'i'); }

	/**
	* Sets the acceleration ramp parameters
	* 
	* @param acc: the acceleration of the motor
	* @param startv: the initial speed
	* @param maxv: the speed at which the motor stops accelerating
	* @param stopv: the final speed
	* @param invertedMovement: if true the movement of the motor
	*	is inverted to take into account the sign of the encoder
	* */
	void setRamp(unsigned int acc, unsigned int startv, unsigned int maxv, unsigned int stopv, bool invertMovement = false);

	/**
	* Returns the status of the locj of the motor
	* 
	* @return lock: if true the motor is locked
	* */
	bool isLocked() { return lock; };

	/**
	* Only used for z movement ( if the movement must be precise )
	* 
	* @param pos: the position to be reached
	* @param v: the speed of the retension
	* */
	void retension(float pos, int v);

	/**
	* Moves the axis at constant speed in the given direction
	* 
	* @param v: the speed of the movement
	* @param d: the direction of the movement
	*	Predefined direction macros: dir_fore, dir_back, dir_up, dir_down
	* */
	void startMeasure(int v = 1, bool d = dir_fore);

	/**
	* Stops the constant movement and
	* locks the motor
	* */
	void stopMeasure();

	static bool measuring;

	/**
	* Another way to call setPosition(B)
	* */
	inline void operator= (double& B) { this -> setPosition(B); }
	
	~Asse();

	unsigned int acceleration;
	unsigned int startV;
	unsigned int maxV;
	unsigned int stopV;

	bool invertedMovement = false;

private:
	/**
	* Performs all the movement operations
	* 
	* @param targetPos: the position to be reached
	* @param instr: 'm'		->	use external instrument coords
	*				default	->	use encoder coords
	* */
	void setPos(double targetPos, const char instr);

	/**
	* Performs the speed calculations while moving
	* the axis
	* */
	void setVelocity(double dist, double trav);

	/**
	* Sends the speed to the microcontroller
	* that sets the frequency of the control signal
	* */
	void sendVelocityToMicro();

	/**
	* Send the commands to the microcontroller
	* to lock/unlock the motor, set the direction
	* */
	void sendCommandToMicro();

	char axisName = x_lab;
	
	bool lock = true;
	bool direction = dir_fore;

	unsigned int velocity = 0;
	double position;

	SimpleSerial ser;
	PosInstr *instrPT;
	PosInstr *measPT = nullptr;
};

