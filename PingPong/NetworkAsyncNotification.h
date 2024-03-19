#pragma once
#include <iostream>
#include <WinSock2.h>
#include "Ball.h"

#pragma comment(lib, "ws2_32.lib")

class NetworkAsyncNotification {
private:
    static constexpr int kBufSize = 13;
    static constexpr int kPort = 7999;
    static constexpr int kMaxPlayers = 2;

    NetworkAsyncNotification(const NetworkAsyncNotification& other);
    NetworkAsyncNotification& operator=(const NetworkAsyncNotification& other);

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
    char buf[kBufSize];
    int pos[3] = { 0,0,0 };

public:
    NetworkAsyncNotification();
    ~NetworkAsyncNotification();
    void HandleEvent(Ball& ball);
    int EventAccept(int sigEventIdx);
    int EventRead(int sigEventIdx, Ball& ball);
    int EventClose(int sigEventIdx);
    void CompressSockets(SOCKET hSockArr[], int idx, int total);
    void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
    void ErrorHandling(const char* buf);
    int GetPosY1();
    int GetPosY2();
};
