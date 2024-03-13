#pragma once
#include <iostream>
#include <thread>
#include <process.h>
#include <mutex>
#include <WinSock2.h>
#include "Ball.h"
#pragma comment(lib, "ws2_32.lib")

#define PORT 7999
#define MAX_PLAYERS 2
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

class Network {
private:
    Network(const Network& other);
    Network& operator=(const Network& other);
    WSADATA wsaData;
    SOCKET listenSocket;
    sockaddr_in serverAddr;
    struct PlayerInfo {
        SOCKET socket;
        int playerID;
    };
    static PlayerInfo players[MAX_PLAYERS];
    static int numPlayers;
    static unsigned __stdcall PlayerThread(void* data);
    static std::mutex playersMutex;
    static std::mutex ballMutex;

public:
    Network();
    ~Network();
    void AcceptPlayer();
};
