#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <chrono>
#include <mutex>
#include <thread>
#include <time.h>

class SimpleSerial
{
private:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	std::string syntax_name_;
	char front_delimiter_;
	char end_delimiter_;

	void CustomSyntax(std::string syntax_type);

public:
	SimpleSerial();
	SimpleSerial(char* com_port, DWORD COM_BAUD_RATE);

	/**
	* Reads the serial port until '\r' is received
	*
	* @param reply_wait_time: The timeout value
	*
	* @return Received_message: The std::string received
	* */
	std::string ReadSerialPort(int reply_wait_time);

	/**
	* Reads the serial port using a specified syntax.
	* The available syntaxes are defined in file syntax_config.txt,
	* New types of syntax can be added.
	*
	* @param reply_wait_time: The timeout value
	* @param syntax_type: The syntax used: (json { } greater_less_than < > square [ ])
	*
	* @return Received_message: The std::string received
	* */
	std::string ReadSerialPort(int reply_wait_time, std::string syntax_type);

	/**
	* Writes a string to the serial port
	*
	* @param data_sent: The c_str to be sent over the serial
	*
	* @return ok_send: Bool that indicates the status of the transmission
	* */
	bool WriteSerialPort(char* data_sent);

	/**
	* Writes a string to the serial port specifying the length
	*
	* @param data_sent: The c_str to be sent over the serial
	* @param n: The length of the c_str
	*
	* @return ok_send: Bool that indicates the status of the transmission
	* */
	bool WriteSerialPort(char* data_sent, unsigned int n);

	/**
	* Opens a serial communication
	*
	* @param com_port: a string that indicates the port name
	* @param COM_BAUD: the baud rate of the serial com
	*
	* @return ok_connect: Bool that indicates the status of the connection
	* */
	bool OpenSerialPort(char* com_port, DWORD COM_BAUD_RATE);

	/**
	* Opens a serial communication
	*
	* @param com_port: a string that indicates the port name
	* @param PORT_PARAMS: all the parameters of the port provided by dcb (winbase.h) 
	* https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-dcb
	*
	* @return ok_connect: Bool that indicates the status of the connection
	* */
	bool OpenSerialPort(char * com_port, DCB PORT_PARAMS);

	/**
	* Closes the serial port
	* */
	bool CloseSerialPort();

	~SimpleSerial();
	bool connected_;
	
	//std::mutex sendMutex;
};

