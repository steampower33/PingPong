#include "Network.h"

int Network::numPlayers = 0;
Network::PlayerInfo Network::players[MAX_PLAYERS];
std::mutex Network::playersMutex;
std::mutex Network::ballMutex;

Network::Network() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // ���� ����
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ���ε�
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

void Network::AcceptPlayer() {
    char isLeftRight[1];
    int leftRight = 0;
    while (true) {
        // �÷��̾� ���� ����
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        // �ִ� �÷��̾� �� �ʰ� �� �ź�
        if (numPlayers >= MAX_PLAYERS) {
            closesocket(clientSocket);
            continue;
        }

        // �÷��̾� ���� ����
        players[numPlayers].socket = clientSocket;
        players[numPlayers].playerID = numPlayers;

        isLeftRight[0] = '0' + leftRight++;
        send(players[numPlayers].socket, isLeftRight, 1, 0);

        // ������ ����
        _beginthreadex(NULL, 0, &PlayerThread, &players[numPlayers], 0, NULL);
        numPlayers++;

        std::cout << "New client connected!" << std::endl;
    }
}

unsigned __stdcall Network::PlayerThread(void* data) {
    PlayerInfo* player = (PlayerInfo*)data;
    char buffer[10];
    int bytesReceived;
    Ball ball;

    while (true) {

        // �÷��̾�κ��� ������ �ޱ�
        bytesReceived = recv(player->socket, buffer, 4, 0);
        if (bytesReceived <= 0) {
            // ���� ���� ó��
            break;
        }

        if (buffer[3] - '0' == 1)
            ball.SetSpeedX(ball.GetSpeedX() * -1);

        ball.Update();
        
        int x = ball.GetX();
        int y = ball.GetY();

        for (int i = 0; i < 4; i++) {
            buffer[6 - i] = '0' + x % 10;
            x /= 10;
            if (i != 3) {
                buffer[9 - i] = '0' + y % 10;
                y /= 10;
            }
        }
        std::cout << player->playerID << ": ";
        for (int i = 0; i < 10; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

        // �÷��̾� ������ ��� Ŭ���̾�Ʈ�� ����
        // ���� �÷��̾� Ŭ���̾�Ʈ -> ���� �÷��̾� ��ġ�� �����ؼ� ����
        {
            std::lock_guard<std::mutex> lock(playersMutex);

            // ���� �÷��̾�� ������ ����
            int otherIndex = (player->playerID == 0) ? 1 : 0;
            send(player[otherIndex].socket, buffer, 10, 0);
        }
    }

    // �÷��̾� ���� ����
    closesocket(player->socket);
    players[player->playerID].socket = INVALID_SOCKET;
    numPlayers--;

    _endthreadex(0);
    return 0;
}
