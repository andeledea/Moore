#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <winsock.h>
#include <WinSock2.h>
//#include <arpa/inet.h>
//#include <netdb.h>

class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    tcp_client();

    /**
    * Connect to a host on a certain port number
    * 
    * @param address: the IP address of the server
    * @param port: the port of the server
    */
    bool conn(std::string address, int port);

    /**
    * Send data to the connected host
    * 
    * @param data: the string to be sent over the TCP/IP
    *     */
    bool send_data(std::string data);

    /**
    * Receive data from the connected host
    */
    std::string receive(int size);
};