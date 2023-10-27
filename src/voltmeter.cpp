#include "voltmeter.h"

void Voltmeter::connect()
{
	handle = ibdev(0, 22, 0, 11, 1, 0x140A);
	std::cout << "Voltmeter connected @ " << handle << std::endl;
}

void Voltmeter::setParams()
{
	ask("BEEP ONCE", 	false);
	ask("ID?", 			true);
	
	ask("NRDGS 10", 	false);  // do 10 readings whenever triggered
	ask("DCV AUTO",		false);  // 10 V range DC
	ask("MATH STAT",	false);  // enable statistics
}

double Voltmeter::readPressure()
{
	ask("TRIG SGL", 	false);  // Trigger the multimeter
	std::this_thread::sleep_for(std::chrono::seconds(2));
	double val = ask("RMATH MEAN", 	true);   // Ask the mean value

	val = val * PRESSURE_A + PRESSURE_B;
	return val;
}

double Voltmeter::readHumidity()  // TODO : change channel
{
	ask("TRIG SGL", 	false);  // Trigger the multimeter
	std::this_thread::sleep_for(std::chrono::seconds(2));
	double val = ask("RMATH MEAN", 	true);   // Ask the mean value

	val = val * HUMIDITY_A + HUMIDITY_B;
	return val;
}

Voltmeter::~Voltmeter()
{
	handle = -1;
}
