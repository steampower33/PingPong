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
    static UINT WINAPI EchoThreadMain(LPVOID CompletionPortIO);
    static HANDLE pos_mutex;
    static HANDLE ball_mutex;
    static HANDLE send_buffer_mutex;
    static HANDLE recv_buffer_mutex;

    typedef struct    // socket info
    {
        SOCKET hClntSock;
        SOCKADDR_IN clntAdr;
    } PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

    typedef struct    // buffer info
    {
        OVERLAPPED overlapped;
        WSABUF wsaBuf;
        char buffer[kBufSize];
        int rwMode;    // READ or WRITE
    } PER_IO_DATA, *LPPER_IO_DATA;

    Network(const Network& other);
    Network& operator=(const Network& other);

public:
    Network();
    ~Network();
    static int pos[2];
    static Ball ball;
    
    void ErrorHandling(const char* buf);
    int GetPosY1();
    int GetPosY2();
};
