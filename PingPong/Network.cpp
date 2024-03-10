#include "Network.h"

int Network::numClients = 0;
Network::ClientInfo Network::clients[MAX_CLIENTS];

Network::Network() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // ���� ����
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ���ε�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // ���� ���
    listen(listenSocket, SOMAXCONN);
    std::cout << "Server started on port " << PORT << std::endl;
}

Network::~Network() {
    // ���� ����
    closesocket(listenSocket);
    WSACleanup();
}

void Network::AcceptClient() {
    char isLeftRight[1];
    int leftRight = 0;
    while (true) {
        // Ŭ���̾�Ʈ ���� ����
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        // �ִ� Ŭ���̾�Ʈ �� �ʰ� �� �ź�
        if (numClients >= MAX_CLIENTS) {
            closesocket(clientSocket);
            continue;
        }

        // Ŭ���̾�Ʈ ���� ����
        clients[numClients].socket = clientSocket;
        clients[numClients].playerID = numClients;

        isLeftRight[0] = '0' + leftRight++;
        send(clients[numClients].socket, isLeftRight, 1, 0);

        // ������ ����
        _beginthreadex(NULL, 0, &ClientThread, &clients[numClients], 0, NULL);
        numClients++;

        std::cout << "New client connected!" << std::endl;
    }
}

unsigned __stdcall Network::ClientThread(void* data) {
    ClientInfo* client = (ClientInfo*)data;
    char buffer[3];
    int bytesReceived;

    while (true) {
        // Ŭ���̾�Ʈ�κ��� ������ �ޱ�
        bytesReceived = recv(client->socket, buffer, 3, 0);
        if (bytesReceived <= 0) {
            // ���� ���� ó��
            break;
        }

        // ���� Ŭ���̾�Ʈ���� ������ ����
        int otherIndex = (client->playerID == 0) ? 1 : 0;
        send(clients[otherIndex].socket, buffer, 3, 0);
    }

    // Ŭ���̾�Ʈ ���� ����
    closesocket(client->socket);
    clients[client->playerID].socket = INVALID_SOCKET;
    numClients--;

    _endthreadex(0);
    return 0;
}