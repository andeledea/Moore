#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>

#include "cary.h"

int Cary::faults = 0;
std::chrono::_V2::system_clock::time_point Cary::start;

Cary::Cary()
{
    faults = 0;
    start = std::chrono::_V2::high_resolution_clock::now();
}

void Cary::connect()
{
    DCB dcbSerialParams = {0};

    dcbSerialParams.BaudRate = CBR_4800;
    dcbSerialParams.ByteSize = 7;
    dcbSerialParams.StopBits = 2;
    dcbSerialParams.Parity = EVENPARITY;

    // Set the lines for the photodiode
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

    char port[] = "COM1";
    if (ser.OpenSerialPort(port, dcbSerialParams)) // apro la com seriale
    {
        std::cout << "[INFO] TESA connected" << std::endl;

        // Do a simple request to start the com
        // this is probably beacuse the port has some init errors
        char toSend[] = "?\r";
        ser.WriteSerialPort(toSend);
        ser.PurgePort();
    }
    else
    {
        std::cout << "[ERROR] TESA NOT connected" << std::endl;
        throw std::runtime_error("Unable to connect TESA");
    }
}

double Cary::readInstr()
{
    char toSend[] = "?\r"; // Request for displayed value.
    ser.PurgePort();
    ser.WriteSerialPort(toSend);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    this->value = ser.ReadSerialPortWithConversion<double>(1, [](const std::string &s)
                                                          { return std::stod(s); });
    if (this->value == 0.0 || this->value > 0.5)
    {
        faults++;
        return this->readInstr();
    }
    return this->value;
}

double Cary::preciseRead(int n_samples_to_read)
{
    double sum = 0.0;
    double average = 0.0;
    double stan_deviation = 0.0;
    double calculate_sd = 0.0;
    double samp_buff[n_samples_to_read];

    for (int i = 0; i < n_samples_to_read; i++)
    {
        samp_buff[i] = this->readInstr();
        sum += samp_buff[i];
    }
    average = sum / (n_samples_to_read);

    for (int i = 0; i < n_samples_to_read; i++)
    {
        stan_deviation += pow(samp_buff[i] - average, 2);
    }
    calculate_sd = sqrt(stan_deviation / (n_samples_to_read));
    if (stan_deviation > 0.0005)
    {
        return this->preciseRead(n_samples_to_read);
    }

    return average;
}

int Cary::getRange()
{
    char toSend[] = "RNG?\r";
    ser.WriteSerialPort(toSend);

    return ser.ReadSerialPortWithConversion<int>(1, [](const std::string &s)
                                                 { return std::stoi(s); });
}

void Cary::setRange(int rng)
{
    std::string tosend = "RNG ";
    tosend += std::to_string(rng);
    tosend += "\r\n";
    ser.WriteSerialPort(tosend.c_str());
}

Cary::~Cary()
{
    std::chrono::duration<double> lifetime = (std::chrono::_V2::high_resolution_clock::now() - start);
    std::cout << "[WARN] Cary faults during object lifetime: " << faults << " / " << lifetime.count() << " = " << faults / (lifetime.count()) << " [faults / s]" << std::endl;
}
