#include <iostream>
#include <fstream>

#include "SimpleSerial.h"

SimpleSerial::SimpleSerial()
{
	connected_ = false;
	io_handler_ = INVALID_HANDLE_VALUE;
}

SimpleSerial::SimpleSerial(char* com_port, DWORD COM_BAUD_RATE)
{
	this->OpenSerialPort(com_port, COM_BAUD_RATE);
}

void SimpleSerial::CustomSyntax(std::string syntax_type) { // json grater_less_than square

	std::ifstream syntaxfile_exist("syntax_config.txt");

	if (!syntaxfile_exist) {
		std::ofstream syntaxfile;
		syntaxfile.open("syntax_config.txt");

		if (syntaxfile) {
			syntaxfile << "json { }\n";
			syntaxfile << "greater_less_than < >\n";
			syntaxfile << "square [ ]\n";
			syntaxfile.close();
		}
	}

	syntaxfile_exist.close();

	std::ifstream syntaxfile_in;
	syntaxfile_in.open("syntax_config.txt");

	std::string line;
	bool found = false;

	if (syntaxfile_in.is_open()) {
		while (syntaxfile_in) {
			syntaxfile_in >> syntax_name_ >> front_delimiter_ >> end_delimiter_;
			getline(syntaxfile_in, line);

			if (syntax_name_ == syntax_type) {
				found = true;
				break;
			}
		}

		syntaxfile_in.close();

		if (!found) {
			syntax_name_ = "";
			front_delimiter_ = ' ';
			end_delimiter_ = ' ';
			printf("Warning: Could not find delimiters, may cause problems!\n");
		}
	}
	else
		throw std::runtime_error("No file custom syntax");
}

std::string SimpleSerial::ReadSerialPort(int reply_wait_time)
{
	DWORD bytes_read;
	char inc_msg[1];
	std::string complete_inc_msg;

	time_t start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);

	while (difftime(time(nullptr), start_time) < reply_wait_time) {
		if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, NULL)) {
			if (inc_msg[0] != '\r') complete_inc_msg.append(inc_msg, 1);
			else return complete_inc_msg;
		}
		else
			return "Warning: Failed to receive data.\n";
	}
	return complete_inc_msg;
}

std::string SimpleSerial::ReadSerialPort(int reply_wait_time, std::string syntax_type) 
{
	//std::lock_guard<std::mutex> lock(sendMutex);	
	
	DWORD bytes_read;
	char inc_msg[1];
	std::string complete_inc_msg;
	bool began = false;

	CustomSyntax(syntax_type);

	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);
	PurgeComm(io_handler_, PURGE_RXCLEAR);

	while ((time(nullptr) - start_time) < reply_wait_time) {
		if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, NULL)) {
			if (inc_msg[0] == front_delimiter_ || began) {
				began = true;

				if (inc_msg[0] == end_delimiter_)
					return complete_inc_msg;

				if (inc_msg[0] != front_delimiter_)
					complete_inc_msg.append(inc_msg, 1);
			}
		}
		else
			return "Warning: Failed to receive data.\n";
	}
	return complete_inc_msg;
}

bool SimpleSerial::WriteSerialPort(char const * data_sent)
{
	//std::lock_guard<std::mutex> lock(sendMutex);
	
	DWORD bytes_sent;

	unsigned int data_sent_length = (unsigned int)strlen(data_sent);

	if (!WriteFile(io_handler_, (void*)data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io_handler_, &errors_, &status_);
		return false;
	}
	else
		return true;
}

bool SimpleSerial::WriteSerialPort(char const * data_sent, unsigned int n)
{
	//const std::lock_guard<std::mutex> lock(sendMutex);
	DWORD bytes_sent;

	unsigned int data_sent_length = n;

	if (!WriteFile(io_handler_, (void*)data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io_handler_, &errors_, &status_);
		return false;
	}
	else
		return true;
}

bool SimpleSerial::OpenSerialPort(char* com_port, DWORD COM_BAUD_RATE)
{
	connected_ = false;

	io_handler_ = CreateFileA(static_cast<LPCSTR>(com_port),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (io_handler_ == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			throw std::runtime_error("COM not connected");
	}
	else {

		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io_handler_, &dcbSerialParams)) {

			throw std::runtime_error("Couldn't get COM parameters");
		}

		else {
			dcbSerialParams.BaudRate = COM_BAUD_RATE;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(io_handler_, &dcbSerialParams))
				throw std::runtime_error("Couldn't set COM parameters");
			else {
				connected_ = true;
				PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
			}
		}
	}
	return connected_;
}

bool SimpleSerial::OpenSerialPort(char* com_port, DCB PORT_PARAMS)
{
	connected_ = false;

	io_handler_ = CreateFileA(static_cast<LPCSTR>(com_port),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (io_handler_ == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			throw std::runtime_error("COM not connected");
	}
	else {
		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io_handler_, &dcbSerialParams)) {

			throw std::runtime_error("Couldn't get COM parameters");
		}

		else {
			dcbSerialParams = PORT_PARAMS;

			if (!SetCommState(io_handler_, &dcbSerialParams))
				throw std::runtime_error("Couldn't set COM parameters");
			else {
				connected_ = true;
				PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
			}
		}
	}
	return connected_;
}

bool SimpleSerial::CloseSerialPort()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
		return true;
	}
	else
		return false;
}

bool SimpleSerial::PurgePort()
{
    return PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

SimpleSerial::~SimpleSerial()
{
	if (connected_) {
		connected_ = false;
		//CloseHandle(io_handler_);
	}
}

