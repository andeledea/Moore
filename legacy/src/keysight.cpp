#include "keysight.h"
#include "E1735A.h"
#include <stdlib.h>
#include <chrono>
#include <thread> 
#include <cmath>


void Keysight::connect()
{
    int ok = Initialize_E1735A_DLL();
    if (ok == 0) {
        E1735A_SelectDevice(0);
    } else {
        std::cerr << "Failed to init" << std::endl;
    }
}

double Keysight::readInstr()
{
    double sample = E1735A_ReadSample() / 2;
    return sample;
}

double Keysight::readSamplesAvg(int n_samples_to_read)
{
    TLaserSample samples[n_samples_to_read];
    
    E1735A_SetupTimer(0.0001);
    E1735A_SetSampleTriggers(TB_SOFTWARE | TB_TIMER);
    E1735A_StartTimer();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    E1735A_StopTimer();

    int num_sample = E1735A_ReadAllSamples(samples, n_samples_to_read);
    double sum = 0.0;
    double readAverageSamples = 0.0;
    double stan_deviation = 0.0;
    
    for(int i = 0; i < n_samples_to_read; i++)
    {
        // std::cout << i << " of " << num_sample << ": " << samples[i].LaserPos << std::endl;
        // std::cout << i << " of " << num_sample << ": " << samples[i].TimeStamp << std::endl;
        sum += samples[i].LaserPos;
    }
    readAverageSamples = (sum)/(n_samples_to_read);
    // std::cout << "[INFO] the average is:" << readAverageSamples<< std::endl;

    for(int j=0; j< n_samples_to_read; j++)
    {
        stan_deviation += pow(samples[j].LaserPos-readAverageSamples, 2);
    }
    stan_deviation = sqrt(stan_deviation / n_samples_to_read);
    if(stan_deviation > 0.001)
    {
        return this->readSamplesAvg(n_samples_to_read);
    }
        
    return readAverageSamples;
}

void Keysight::setTemp(double T)
{
    if(!E1735A_SetParameter(OP_AIRTEMP, T)) {
        std::cerr << "Error in setting temp to E1735A" << std::endl;
        return;
    }
}

void Keysight::setPress(double P)
{
    if(!E1735A_SetParameter(OP_AIRPRES, P)) {
        std::cerr << "Error in setting temp to E1735A" << std::endl;
        return;
    }
}

void Keysight::setwave_len(double W)
{
    if(!E1735A_SetParameter(OP_WAVELENGTH, W)) {
        std::cerr << "Error in setting temp to E1735A" << std::endl;
        return;
    }
}

void Keysight::setHum(double H)
{
    if(!E1735A_SetParameter(OP_RELHUMI, H)) {
        std::cerr << "Error in setting temp to E1735A" << std::endl;
        return;
    }
}

void Keysight::setDeadP(double D)
{
    if(!E1735A_SetParameter(OP_DEADPATH, D)) {
        std::cerr << "Error in setting temp to E1735A" << std::endl;
        return;
    }
}
