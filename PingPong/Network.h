#pragma once
#include <iostream>
#include <WinSock2.h>
#include <string.h>
#include "Ball.h"

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 13
#define PORT 7999
#define MAX_PLAYERS 2
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

class Network {
private:
    Network(const Network& other);
    Network& operator=(const Network& other);

    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAdr, clntAdr;
    SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT newEvent;
    WSANETWORKEVENTS netEvents;

    int numOfClntSock = 0;
    int strLen, i;
    int posInfo, startIdx;
    int clntAdrLen;
    char buf[BUF_SIZE];
    int pos[3] = { 0,0,0 };

public:
    Network();
    ~Network();
    void HandleEvent(Ball& ball);
    void CompressSockets(SOCKET hSockArr[], int idx, int total);
    void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
    void ErrorHandling(const char* buf);
    int GetPosY1();
    int GetPosY2();
};
