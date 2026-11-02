#include "voltmeter.h"

void Voltmeter::connect()
{
	handle = ibdev(0, 22, 0, 11, 1, 0x140A);
	std::cout << "[INFO] Voltmeter connected @ " << handle << std::endl;
}

void Voltmeter::setParams()
{
	ask("BEEP ONCE", 	false);
	ask("ID?", 			true);
	
	ask("NRDGS 10", 	false);  // do 10 readings whenever triggered
	ask("DCV AUTO",		false);  // 10 V range DC
	// ask("DISP MSG,'INRIM'", false);
}

double Voltmeter::readPressure()
{
	ask("MATH STAT", false);  // enable statistics
	ask("TERM REAR", false);
	ask("CHAN 5", false);
	ask("TRIG SGL", false);  // Trigger the multimeter
	std::this_thread::sleep_for(std::chrono::seconds(5));
	double val = ask("RMATH MEAN", true);   // Ask the mean value

	val = val * PRESSURE_A + PRESSURE_B;
	return val;
}

double Voltmeter::readHumidity()  // TODO : change channel
{
	ask("MATH STAT", false);  // enable statistics
	ask("TERM REAR", false);
	ask("CHAN 0", false);
	ask("TRIG SGL", false);  // Trigger the multimeter
	std::this_thread::sleep_for(std::chrono::seconds(5));
	double val = ask("RMATH MEAN", true);   // Ask the mean value

	val = val * HUMIDITY_A + HUMIDITY_B;

	return val;
}

Voltmeter::~Voltmeter()
{
	handle = -1;
}
