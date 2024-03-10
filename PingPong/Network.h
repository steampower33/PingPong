#pragma once
#include <iostream>
#include <WinSock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 7999
#define MAX_CLIENTS 2

class Network {
private:
    Network(const Network& other);
    Network& operator=(const Network& other);
    SOCKET listenSocket;
    struct ClientInfo {
        SOCKET socket;
        int playerID;
    };
    static ClientInfo clients[MAX_CLIENTS];
    static int numClients;

public:
    Network();
    ~Network();
    void AcceptClient();
    static unsigned __stdcall ClientThread(void* data);
    
};
