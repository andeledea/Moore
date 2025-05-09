#include "client.h"

tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    address = "";
}

bool tcp_client::conn(std::string address, int port)
{
    //create socket if it is not already created
    if (sock == -1)
    {
        //Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        std::cout << "[INFO] Socket created\n";
    }
    else { /* OK , nothing */ }

    // setup address structure
    if (inet_addr(address.c_str()) == -1)
    {
        struct hostent* he;
        struct in_addr** addr_list;

        //resolve the hostname, its not an ip address
        if ((he = gethostbyname(address.c_str())) == NULL)
        {
            //gethostbyname failed
            perror("gethostbyname");
            std::cout << "[ERROR] Failed to resolve hostname\n";

            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr**)he->h_addr_list;

        for (int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];

            std::cout << address << " resolved to " << inet_ntoa(*addr_list[i]) << std::endl;

            break;
        }
    }

    // plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //Connect to remote server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return false;
    }

    std::cout << "[INFO] Client connected\n";
    return true;
}


bool tcp_client::send_data(std::string data)
{
    //Send some data
    if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }

    return true;
}


std::string tcp_client::receive(int size)
{
    char buffer[size];
	
	//Receive a reply from the server
    std::string reply;

    int nch = recv(sock, buffer, sizeof(buffer), 0);
	if (nch < 0)
	{
		puts("Receive failed");
	}
	
    buffer[nch] = '\0';
    reply = buffer;
    return reply;
}

tcp_client::~tcp_client()
{
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
    }
}

