#include <iostream>
#include <WinSock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 7999
#define MAX_CLIENTS 2

// 클라이언트 정보 구조체
struct ClientInfo {
    SOCKET socket;
    int playerID;
};

// 전역 변수
SOCKET listenSocket;
ClientInfo clients[MAX_CLIENTS];
int numClients = 0;

// 스레드 함수
unsigned __stdcall ClientThread(void* data) {
    ClientInfo* client = (ClientInfo*)data;
    char buffer[3];
    int bytesReceived;

    while (true) {
        // 클라이언트로부터 데이터 받기
        bytesReceived = recv(client->socket, buffer, 3, 0);
        if (bytesReceived <= 0) {
            // 연결 끊김 처리
            break;
        }

        // 상대방 클라이언트에게 데이터 전송
        int otherIndex = (client->playerID == 0) ? 1 : 0;
        send(clients[otherIndex].socket, buffer, 3, 0);
    }

    // 클라이언트 연결 종료
    closesocket(client->socket);
    clients[client->playerID].socket = INVALID_SOCKET;
    numClients--;

    _endthreadex(0);
    return 0;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 소켓 생성
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 바인딩
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // 수신 대기
    listen(listenSocket, SOMAXCONN);
    std::cout << "Server started on port " << PORT << std::endl;

    char isLeftRight[1];
    isLeftRight[0] = '0';
    int leftRight = 0;
    while (true) {
        // 클라이언트 연결 수락
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        // 최대 클라이언트 수 초과 시 거부
        if (numClients >= MAX_CLIENTS) {
            closesocket(clientSocket);
            continue;
        }

        // 클라이언트 정보 저장
        clients[numClients].socket = clientSocket;
        clients[numClients].playerID = numClients;

        isLeftRight[0] = '0' + leftRight++;
        send(clients[numClients].socket, isLeftRight, 1, 0);

        // 스레드 생성
        _beginthreadex(NULL, 0, &ClientThread, &clients[numClients], 0, NULL);
        numClients++;

        std::cout << "New client connected!" << std::endl;
    }

    // 소켓 해제
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}