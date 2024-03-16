#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "Ball.h"
#include "Enemy.h"

#pragma comment(lib, "ws2_32.lib")

class Network {
private:
    static constexpr char kServerIp[] = "127.0.0.1";
    static constexpr int kServerPort = 7999;
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[10];
    int bytesReceived;
    int playerY;
    int ballX;
    int ballY;
public:
    Network();
    ~Network();
    void sendData(int y, Ball& ball);
    int recvPositionData(int dataSize);
    void recvData(int dataSize, Ball& ball, Enemy& enemy);
    void ErrorHandling(const char* buf);
};
