#ifndef _KEY_K
#define _KEY_K

#include <fstream>
#include <windows.h>
#include <tchar.h>

#include "PosInstr.h"

#define OP_WAVELENGTH 0
#define OP_AIRTEMP 1
#define OP_AIRPRES 2
#define OP_RELHUMI 3
#define OP_DEADPATH 16

class Keysight : PosInstr
{
public:
    void connect();

    /**
     * @brief returns the double value of the sample read 
     * from the function
     */
    double readInstr();

    /**
     * @brief 
     * @param calculates the average of 10 samples and calculates the standard deviation
     * if the standard deviation < 0.001 it re-reads the samples
     */

    double readSamplesAvg(int n_samples_to_read);

    /**
     * @brief sets the temperature to the keysight interface
     * @param T: double, the temperature value
     * */ 
    void setTemp(double T);
    /**
     * @brief sets the pressure to the keysight interface
     * @param P: double, the pressure value
     */
    void setPress(double P);

    /**
     *@brief sets the wave_length to the keysight interface
     @param W : double, the wavelength value 
     */
    void setwave_len(double W);

    /**
     * @brief sets the humidity to the keysight interface
     * @param H: double, the humidity value
     */
    void setHum(double H);

    /**
     * @brief sets the deadpath to the keysight interface
     * @param D: double, the deadpath value (linear plane mirror) pg37 pdf
     */
    void setDeadP(double D);
};
#endif