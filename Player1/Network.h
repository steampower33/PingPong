#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7999

#if defined(_MSC_VER) || defined(__BORLANDC__)
#pragma comment(lib, "ws2_32.lib")
#endif

class Network {
public:
    Network();
    ~Network();
    void sendData(int y);
    float recvData(int dataSize);
    
private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[4];
    int bytesReceived;
};

#endif