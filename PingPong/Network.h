#pragma once
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include "Ball.h"

#pragma comment(lib, "ws2_32.lib")

class Network {
private:
    static constexpr int kBufSize = 12;
    static constexpr int kPort = 7999;
    static constexpr int kMaxPlayers = 2;
    static constexpr int kRead = 3;
    static constexpr int kWrite = 5;
    static int pos[2];
    static Ball ball;
    static HANDLE pos_mutex;
    static HANDLE ball_mutex;
    static HANDLE send_buffer_mutex;
    static HANDLE recv_buffer_mutex;
    static UINT WINAPI EchoThreadMain(LPVOID CompletionPortIO);

    typedef struct    // socket info
    {
        SOCKET hClntSock;
        SOCKADDR_IN clntAdr;
    } PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

    typedef struct    // buffer info
    {
        OVERLAPPED overlapped;
        WSABUF wsaBuf;
        char buffer[kBufSize];
        int rwMode;    // READ or WRITE
    } PER_IO_DATA, * LPPER_IO_DATA;

    int numOfClntSock, addrLen;
    WSADATA	wsaData;
    HANDLE hComPort;
    SYSTEM_INFO sysInfo;
    SOCKET hServSock;
    SOCKADDR_IN servAdr;
    SOCKET hClntSock;
    SOCKADDR_IN clntAdr;
    LPPER_HANDLE_DATA handleInfo;
    LPPER_IO_DATA ioInfo;

    Network(const Network& other);
    Network& operator=(const Network& other);
    void ErrorHandling(const char* buf);

public:
    Network();
    ~Network();
    
    void AcceptClient();
    void SendPositionToClient();
    void ConnectClntSockToCP();
    void RecvData();
};
