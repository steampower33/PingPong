#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "Ball.h"
#include "Enemy.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7999

#if defined(_MSC_VER) || defined(__BORLANDC__)
#pragma comment(lib, "ws2_32.lib")
#endif

class Network {
public:
    Network();
    ~Network();
    void sendData(int y, Ball& ball);
    int recvPositionData(int dataSize);
    void recvData(int dataSize, Ball& ball, Enemy& enemy);
    
private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[10];
    int bytesReceived;
    int playerY;
    int ballX;
    int ballY;
};
